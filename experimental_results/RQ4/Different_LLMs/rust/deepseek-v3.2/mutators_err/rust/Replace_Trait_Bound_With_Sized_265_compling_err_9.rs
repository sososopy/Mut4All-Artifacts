use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemImpl, ImplItem, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Trait_Bound_With_Sized_265;

impl Mutator for Replace_Trait_Bound_With_Sized_265 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Sized_265"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                syn::Item::Fn(func) => {
                    self.mutate_generics(&mut func.sig.generics);
                }
                syn::Item::Struct(struct_item) => {
                    self.mutate_generics(&mut struct_item.generics);
                }
                syn::Item::Enum(enum_item) => {
                    self.mutate_generics(&mut enum_item.generics);
                }
                syn::Item::Impl(impl_item) => {
                    self.mutate_generics(&mut impl_item.generics);
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            self.mutate_generics(&mut method.sig.generics);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds in where clauses across generic definitions (structs, enums, functions, impls). It replaces a trait bound (e.g., T: SomeTrait) with a Sized bound on the same type parameter if that parameter does not already have an explicit Sized bound. If the seed already extensively uses Sized bounds, it adds an extra redundant Sized bound for a different type parameter. This transformation introduces potentially conflicting or overly restrictive Sized requirements, aiming to expose compiler assumptions about trait satisfaction and sizedness during monomorphization or trait selection, potentially triggering out-of-bounds errors in internal data structures."
    }
}

impl Replace_Trait_Bound_With_Sized_265 {
    fn mutate_generics(&self, generics: &mut syn::Generics) {
        if let Some(where_clause) = &mut generics.where_clause {
            let mut predicates_to_modify = Vec::new();
            let mut sized_bounds = HashSet::new();
            
            for predicate in &where_clause.predicates {
                if let syn::WherePredicate::Type(predicate_type) = predicate {
                    if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                        if let Some(ident) = type_path.path.get_ident() {
                            for bound in &predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.segments.last().map(|seg| seg.ident == "Sized").unwrap_or(false) {
                                        sized_bounds.insert(ident.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            for predicate in &where_clause.predicates {
                if let syn::WherePredicate::Type(predicate_type) = predicate {
                    if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                        if let Some(ident) = type_path.path.get_ident() {
                            let has_trait_bound = predicate_type.bounds.iter().any(|bound| {
                                match bound {
                                    syn::TypeParamBound::Trait(trait_bound) => !trait_bound.path.segments.last().map(|seg| seg.ident == "Sized").unwrap_or(false),
                                    _ => false,
                                }
                            });
                            if has_trait_bound && !sized_bounds.contains(&ident) {
                                predicates_to_modify.push(ident.clone());
                            }
                        }
                    }
                }
            }
            
            if predicates_to_modify.is_empty() {
                let mut type_params = Vec::new();
                for param in &generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        type_params.push(type_param.ident.clone());
                    }
                }
                if !type_params.is_empty() {
                    let mut rng = thread_rng();
                    if let Some(target_param) = type_params.choose(&mut rng) {
                        if !sized_bounds.contains(target_param) {
                            predicates_to_modify.push(target_param.clone());
                        }
                    }
                }
            }
            
            for ident in predicates_to_modify {
                let mut new_predicates = Punctuated::new();
                for predicate in &mut where_clause.predicates {
                    if let syn::WherePredicate::Type(predicate_type) = predicate {
                        if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                            if let Some(predicate_ident) = type_path.path.get_ident() {
                                if predicate_ident == &ident {
                                    let mut new_bounds = Punctuated::new();
                                    new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path::from(syn::Ident::new("Sized", Span::call_site())),
                                    }));
                                    predicate_type.bounds = new_bounds;
                                }
                            }
                        }
                    }
                    new_predicates.push(predicate.clone());
                }
                where_clause.predicates = new_predicates;
            }
        }
    }
}