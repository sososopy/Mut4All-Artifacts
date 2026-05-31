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

pub struct Replace_Placeholder_Lifetime_In_Associated_Type_Bounds_295;

impl Mutator for Replace_Placeholder_Lifetime_In_Associated_Type_Bounds_295 {
    fn name(&self) -> &str {
        "Replace_Placeholder_Lifetime_In_Associated_Type_Bounds_295"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = PlaceholderLifetimeReplacer::default();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method signatures with return position impl trait (RPITIT) containing associated type bounds with placeholder lifetimes ('_). It replaces '_ with an existing lifetime parameter from the method's input parameters (e.g., &self lifetime). If no suitable lifetime exists, it adds a new lifetime parameter to the method signature. This transformation forces lifetime resolution across trait and opaque type scopes, potentially exposing bugs in associated type bounds and lifetime boundary mismatches."
    }
}

#[derive(Default)]
struct PlaceholderLifetimeReplacer {
    current_method_lifetimes: Vec<String>,
}

impl VisitMut for PlaceholderLifetimeReplacer {
    fn visit_item_trait_mut(&mut self, node: &mut ItemTrait) {
        for item in &mut node.items {
            if let TraitItem::Fn(method) = item {
                self.process_trait_method(method);
            }
        }
        syn::visit_mut::visit_item_trait_mut(self, node);
    }
}

impl PlaceholderLifetimeReplacer {
    fn process_trait_method(&mut self, method: &mut TraitItemFn) {
        self.current_method_lifetimes.clear();
        
        for param in &method.sig.generics.params {
            if let GenericParam::Lifetime(lifetime_param) = param {
                self.current_method_lifetimes.push(lifetime_param.lifetime.ident.to_string());
            }
        }
        
        for input in &method.sig.inputs {
            self.collect_lifetimes_from_input(input);
        }
        
        let sig_copy = method.sig.clone();
        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
            self.replace_placeholder_in_type(return_type, &sig_copy);
        }
    }
    
    fn collect_lifetimes_from_input(&mut self, input: &FnArg) {
        match input {
            FnArg::Receiver(receiver) => {
                if let Some(lifetime) = receiver.lifetime() {
                    let lifetime_str = lifetime.ident.to_string();
                    if !self.current_method_lifetimes.contains(&lifetime_str) {
                        self.current_method_lifetimes.push(lifetime_str);
                    }
                }
            }
            FnArg::Typed(pat_type) => {
                let mut ty = pat_type.ty.clone();
                self.visit_type_mut(&mut ty);
            }
        }
    }
    
    fn replace_placeholder_in_type(&mut self, ty: &mut Box<Type>, sig: &Signature) {
        match &mut **ty {
            Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_impl_trait.bounds {
                    self.process_type_param_bound(bound, sig);
                }
            }
            Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(arg_ty) = arg {
                                let mut boxed = Box::new(arg_ty.clone());
                                self.replace_placeholder_in_type(&mut boxed, sig);
                                *arg = GenericArgument::Type(*boxed);
                            } else if let GenericArgument::AssocType(binding) = arg {
                                if let Type::TraitObject(trait_obj) = &mut binding.ty {
                                    for bound in &mut trait_obj.bounds {
                                        self.process_type_param_bound(bound, sig);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Type::TraitObject(trait_obj) => {
                for bound in &mut trait_obj.bounds {
                    self.process_type_param_bound(bound, sig);
                }
            }
            _ => {}
        }
    }
    
    fn process_type_param_bound(&mut self, bound: &mut TypeParamBound, sig: &Signature) {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if let Some(lifetimes) = &mut trait_bound.lifetimes {
                for lifetime_bound in &mut lifetimes.lifetimes {
                    if let GenericParam::Lifetime(lifetime_param) = lifetime_bound {
                        if lifetime_param.lifetime.ident == "_" {
                            if let Some(replacement) = self.select_replacement_lifetime(sig) {
                                *lifetime_bound = GenericParam::Lifetime(LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: replacement,
                                    colon_token: token::Colon::default(),
                                    bounds: Punctuated::new(),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn select_replacement_lifetime(&self, sig: &Signature) -> Option<Lifetime> {
        if !self.current_method_lifetimes.is_empty() {
            let selected = self.current_method_lifetimes.first().unwrap();
            return Some(Lifetime::new(&format!("'{}", selected), Span::call_site()));
        }
        
        for input in &sig.inputs {
            if let FnArg::Receiver(receiver) = input {
                if receiver.reference.is_some() && receiver.lifetime().is_none() {
                    return Some(Lifetime::new("'a", Span::call_site()));
                }
            }
        }
        
        None
    }
    
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::Reference(type_ref) => {
                if let Some(lifetime) = &type_ref.lifetime {
                    let lifetime_str = lifetime.ident.to_string();
                    if !self.current_method_lifetimes.contains(&lifetime_str) {
                        self.current_method_lifetimes.push(lifetime_str);
                    }
                }
            }
            Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                let lifetime_str = lifetime.ident.to_string();
                                if !self.current_method_lifetimes.contains(&lifetime_str) {
                                    self.current_method_lifetimes.push(lifetime_str);
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}