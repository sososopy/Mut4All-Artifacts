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

pub struct Replace_Const_Assoc_Bound_With_Invalid_294;

impl Mutator for Replace_Const_Assoc_Bound_With_Invalid_294 {
    fn name(&self) -> &str {
        "Replace_Const_Assoc_Bound_With_Invalid_294"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_type_needed = false;
        let mut visitor = ConstAssocVisitor {
            dummy_type_needed: &mut dummy_type_needed,
        };
        visitor.visit_file_mut(file);
        
        if dummy_type_needed {
            let dummy_struct: Item = parse_quote! {
                struct SomeType;
            };
            file.items.insert(0, dummy_struct);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets associated type bounds where the associated item is actually a constant rather than a type. It replaces the bound with a type bound referencing a non-existent type `SomeType`, creating a type mismatch. If `SomeType` doesn't exist in scope, a dummy struct is added at the module level. This transformation aims to test the compiler's handling of const-associated items in trait bounds and its error recovery for invalid type bounds."
    }
}

struct ConstAssocVisitor<'a> {
    dummy_type_needed: &'a mut bool,
}

impl<'a> VisitMut for ConstAssocVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::ImplTrait(type_impl_trait) => {
                self.process_bounds(&mut type_impl_trait.bounds);
            }
            Type::TraitObject(type_trait_object) => {
                self.process_bounds(&mut type_trait_object.bounds);
            }
            Type::Path(type_path) => {
                self.visit_type_path_mut(type_path);
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_type_path_mut(&mut self, type_path: &mut TypePath) {
        if let Some(qself) = &mut type_path.qself {
            self.visit_type_mut(&mut qself.ty);
        }
        
        for segment in &mut type_path.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::AssocType(assoc_type) = arg {
                        if let Type::Path(type_path) = &mut assoc_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                let path_str = segment.ident.to_string();
                                if path_str.contains("literal") || path_str.contains("const") {
                                    *self.dummy_type_needed = true;
                                    let new_path: SynPath = parse_quote! { SomeType };
                                    type_path.path = new_path;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn visit_trait_bound_mut(&mut self, trait_bound: &mut TraitBound) {
        let path_str = format!("{}", quote! { #trait_bound });
        if path_str.contains("<") && (path_str.contains("literal") || path_str.contains("const")) {
            if let Some(segment) = trait_bound.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::AssocType(assoc_type) = arg {
                            if let Type::Path(type_path) = &mut assoc_type.ty {
                                let path_str = format!("{}", quote! { #type_path });
                                if path_str.contains("literal") || path_str.contains("const") {
                                    *self.dummy_type_needed = true;
                                    let new_path: SynPath = parse_quote! { SomeType };
                                    type_path.path = new_path;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_trait_bound_mut(self, trait_bound);
    }
}

impl<'a> ConstAssocVisitor<'a> {
    fn process_bounds(&mut self, bounds: &mut Punctuated<TypeParamBound, token::Plus>) {
        for bound in bounds.iter_mut() {
            if let TypeParamBound::Trait(trait_bound) = bound {
                self.visit_trait_bound_mut(trait_bound);
            }
        }
    }
}