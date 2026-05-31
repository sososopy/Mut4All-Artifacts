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

pub struct Replace_Simple_Trait_Bound_With_For_Binder_435;

impl Mutator for Replace_Simple_Trait_Bound_With_For_Binder_435 {
    fn name(&self) -> &str {
        "Replace_Simple_Trait_Bound_With_For_Binder_435"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                if type_path.path.segments.len() == 1 {
                                    let segment = &type_path.path.segments[0];
                                    if segment.ident != "Self" && !segment.ident.to_string().starts_with("__") {
                                        if type_pred.bounds.len() == 1 {
                                            if let syn::TypeParamBound::Trait(trait_bound) = &type_pred.bounds[0] {
                                                let mut for_generics = Generics::default();
                                                for_generics.params.push(parse_quote!(const N: usize));
                                                for_generics.params.push(parse_quote!(const NP: usize = { N + 1 }));
                                                let new_predicate = syn::WherePredicate::For(parse_quote!(for <#for_generics> #type_pred));
                                                *predicate = new_predicate;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(type_pred) = predicate {
                                    if let syn::Type::Path(type_path) = &type_pred.bounded_ty {
                                        if type_path.path.segments.len() == 1 {
                                            let segment = &type_path.path.segments[0];
                                            if segment.ident != "Self" && !segment.ident.to_string().starts_with("__") {
                                                if type_pred.bounds.len() == 1 {
                                                    if let syn::TypeParamBound::Trait(trait_bound) = &type_pred.bounds[0] {
                                                        let mut for_generics = Generics::default();
                                                        for_generics.params.push(parse_quote!(const N: usize));
                                                        for_generics.params.push(parse_quote!(const NP: usize = { N + 1 }));
                                                        let new_predicate = syn::WherePredicate::For(parse_quote!(for <#for_generics> #type_pred));
                                                        *predicate = new_predicate;
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}