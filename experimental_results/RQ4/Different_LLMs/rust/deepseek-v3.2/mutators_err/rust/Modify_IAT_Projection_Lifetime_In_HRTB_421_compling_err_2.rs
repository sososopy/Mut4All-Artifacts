use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_IAT_Projection_Lifetime_In_HRTB_421;

impl Mutator for Modify_IAT_Projection_Lifetime_In_HRTB_421 {
    fn name(&self) -> &str {
        "Modify_IAT_Projection_Lifetime_In_HRTB_421"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = IatProjectionLifetimeVisitor {
            rng: thread_rng(),
            modified: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated type projections nested within higher-ranked trait bounds and function pointer types. It modifies lifetimes used in associated type projections within for<'a> fn(...) signatures, changing them to either 'static, existing named lifetimes from outer scopes, or introducing new lifetime parameters. This creates complex lifetime dependencies that stress the compiler's trait selection and subtype checking logic for inherent associated types and higher-ranked bounds."
    }
}

struct IatProjectionLifetimeVisitor {
    rng: rand::rngs::ThreadRng,
    modified: bool,
}

impl VisitMut for IatProjectionLifetimeVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(TypePath { path, qself: None }) = ty {
            self.visit_path_mut(path);
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        if let Some(last_segment) = path.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(inner_ty) = arg {
                        self.visit_type_mut(inner_ty);
                    }
                }
            }
        }

        if path.segments.len() >= 2 {
            let last_two = &path.segments[path.segments.len() - 2..];
            if last_two[1].ident == "Assoc" {
                if let PathArguments::AngleBracketed(args) = &last_two[0].arguments {
                    for arg in &args.args {
                        if let GenericArgument::Type(Type::Path(TypePath { path: inner_path, qself: None })) = arg {
                            if let Some(inner_seg) = inner_path.segments.last() {
                                if inner_seg.ident == "fn" {
                                    if let PathArguments::AngleBracketed(fn_args) = &inner_seg.arguments {
                                        for fn_arg in &fn_args.args {
                                            if let GenericArgument::Type(fn_ty) = fn_arg {
                                                if let Type::Reference(ref_ty) = fn_ty {
                                                    if let Some(lifetime) = &ref_ty.lifetime {
                                                        let new_lifetime = self.select_new_lifetime(lifetime);
                                                        if new_lifetime.ident != lifetime.ident {
                                                            let mut new_ref_ty = ref_ty.clone();
                                                            new_ref_ty.lifetime = Some(new_lifetime);
                                                            *fn_ty = Type::Reference(new_ref_ty);
                                                            self.modified = true;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }

    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        if !self.modified {
            let mut lifetimes_in_scope = HashSet::new();
            self.collect_lifetimes_in_fn(&i.sig, &mut lifetimes_in_scope);
            
            for input in &mut i.sig.inputs {
                if let FnArg::Typed(PatType { ty, .. }) = input {
                    self.visit_type_with_scope_mut(ty, &lifetimes_in_scope);
                }
            }
            
            if let ReturnType::Type(_, ty) = &mut i.sig.output {
                self.visit_type_with_scope_mut(ty, &lifetimes_in_scope);
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
}

impl IatProjectionLifetimeVisitor {
    fn collect_lifetimes_in_fn(&mut self, sig: &syn::Signature, set: &mut HashSet<String>) {
        for param in &sig.generics.params {
            if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = param {
                set.insert(lifetime.ident.to_string());
            }
        }
        
        for input in &sig.inputs {
            if let FnArg::Typed(PatType { ty, .. }) = input {
                self.collect_lifetimes_in_type(ty, set);
            }
        }
        
        if let ReturnType::Type(_, ty) = &sig.output {
            self.collect_lifetimes_in_type(ty, set);
        }
    }
    
    fn collect_lifetimes_in_type(&mut self, ty: &Type, set: &mut HashSet<String>) {
        match ty {
            Type::Reference(ref_ty) => {
                if let Some(lifetime) = &ref_ty.lifetime {
                    set.insert(lifetime.ident.to_string());
                }
                self.collect_lifetimes_in_type(&ref_ty.elem, set);
            }
            Type::Path(TypePath { path, .. }) => {
                for segment in &path.segments {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            match arg {
                                GenericArgument::Type(inner_ty) => {
                                    self.collect_lifetimes_in_type(inner_ty, set);
                                }
                                GenericArgument::Lifetime(lifetime) => {
                                    set.insert(lifetime.ident.to_string());
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
            Type::Slice(slice_ty) => {
                self.collect_lifetimes_in_type(&slice_ty.elem, set);
            }
            Type::Array(array_ty) => {
                self.collect_lifetimes_in_type(&array_ty.elem, set);
            }
            Type::Ptr(ptr_ty) => {
                self.collect_lifetimes_in_type(&ptr_ty.elem, set);
            }
            Type::BareFn(bare_fn) => {
                for input in &bare_fn.inputs {
                    self.collect_lifetimes_in_type(&input.ty, set);
                }
                if let ReturnType::Type(_, ret_ty) = &bare_fn.output {
                    self.collect_lifetimes_in_type(ret_ty, set);
                }
            }
            Type::TraitObject(trait_obj) => {
                for bound in &trait_obj.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(lifetimes) = &trait_bound.lifetimes {
                            for lifetime in &lifetimes.lifetimes {
                                set.insert(lifetime.ident.to_string());
                            }
                        }
                    }
                }
            }
            Type::ImplTrait(impl_trait) => {
                for bound in &impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(lifetimes) = &trait_bound.lifetimes {
                            for lifetime in &lifetimes.lifetimes {
                                set.insert(lifetime.ident.to_string());
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
    
    fn visit_type_with_scope_mut(&mut self, ty: &mut Type, lifetimes_in_scope: &HashSet<String>) {
        match ty {
            Type::Reference(ref_ty) => {
                if let Some(lifetime) = &ref_ty.lifetime {
                    let new_lifetime = self.select_lifetime_from_set(lifetime, lifetimes_in_scope);
                    if new_lifetime.ident != lifetime.ident {
                        let mut new_ref_ty = ref_ty.clone();
                        new_ref_ty.lifetime = Some(new_lifetime);
                        let elem = std::mem::replace(&mut ref_ty.elem, parse_quote!(()));
                        *ty = Type::Reference(new_ref_ty);
                        self.visit_type_with_scope_mut(&mut new_ref_ty.elem, lifetimes_in_scope);
                        self.modified = true;
                        return;
                    }
                }
                self.visit_type_with_scope_mut(&mut ref_ty.elem, lifetimes_in_scope);
            }
            Type::Path(TypePath { path, .. }) => {
                self.visit_path_with_scope_mut(path, lifetimes_in_scope);
            }
            Type::Slice(slice_ty) => {
                self.visit_type_with_scope_mut(&mut slice_ty.elem, lifetimes_in_scope);
            }
            Type::Array(array_ty) => {
                self.visit_type_with_scope_mut(&mut array_ty.elem, lifetimes_in_scope);
            }
            Type::Ptr(ptr_ty) => {
                self.visit_type_with_scope_mut(&mut ptr_ty.elem, lifetimes_in_scope);
            }
            Type::BareFn(bare_fn) => {
                for input in &mut bare_fn.inputs {
                    self.visit_type_with_scope_mut(&mut input.ty, lifetimes_in_scope);
                }
                if let ReturnType::Type(_, ret_ty) = &mut bare_fn.output {
                    self.visit_type_with_scope_mut(ret_ty, lifetimes_in_scope);
                }
            }
            _ => {}
        }
    }
    
    fn visit_path_with_scope_mut(&mut self, path: &mut SynPath, lifetimes_in_scope: &HashSet<String>) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    match arg {
                        GenericArgument::Type(inner_ty) => {
                            self.visit_type_with_scope_mut(inner_ty, lifetimes_in_scope);
                        }
                        GenericArgument::Lifetime(lifetime) => {
                            let new_lifetime = self.select_lifetime_from_set(lifetime, lifetimes_in_scope);
                            if new_lifetime.ident != lifetime.ident {
                                *lifetime = new_lifetime;
                                self.modified = true;
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    
    fn select_new_lifetime(&mut self, current: &Lifetime) -> Lifetime {
        let choices = vec![
            Lifetime::new("'static", current.span()),
            Lifetime::new("'a", current.span()),
            Lifetime::new("'b", current.span()),
            Lifetime::new("'c", current.span()),
            Lifetime::new("'d", current.span()),
        ];
        choices.choose(&mut self.rng).unwrap_or(&Lifetime::new("'static", current.span())).clone()
    }
    
    fn select_lifetime_from_set(&mut self, current: &Lifetime, set: &HashSet<String>) -> Lifetime {
        let mut candidates: Vec<String> = set.iter().cloned().collect();
        candidates.push("'static".to_string());
        candidates.push("'a".to_string());
        candidates.push("'b".to_string());
        candidates.push("'c".to_string());
        candidates.push("'d".to_string());
        
        if let Some(choice) = candidates.choose(&mut self.rng) {
            Lifetime::new(choice, current.span())
        } else {
            Lifetime::new("'static", current.span())
        }
    }
}