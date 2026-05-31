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

pub struct Flip_Const_Comparison_In_Trait_Impl_102;

impl Mutator for Flip_Const_Comparison_In_Trait_Impl_102 {
    fn name(&self) -> &str {
        "Flip_Const_Comparison_In_Trait_Impl_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(ref mut where_clause) = impl_item.generics.where_clause {
                    let predicates = &mut where_clause.predicates;
                    let mut candidates = Vec::new();
                    for predicate in predicates.iter() {
                        if let syn::WherePredicate::Type(pred_type) = predicate {
                            for bound in pred_type.bounds.iter() {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(path) = trait_bound.path.segments.last() {
                                        if path.ident == "eq" || path.ident == "ne" {
                                            candidates.push(predicate);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if candidates.len() > 0 {
                        let target = candidates.choose(&mut rng).unwrap();
                        let target_idx = predicates.iter().position(|p| p == *target).unwrap();
                        if let syn::WherePredicate::Type(pred_type) = &mut predicates[target_idx] {
                            for bound in pred_type.bounds.iter_mut() {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(path) = trait_bound.path.segments.last_mut() {
                                        if path.ident == "eq" {
                                            path.ident = Ident::new("ne", Span::call_site());
                                        } else if path.ident == "ne" {
                                            path.ident = Ident::new("eq", Span::call_site());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(ref mut where_clause) = func.sig.generics.where_clause {
                            let predicates = &mut where_clause.predicates;
                            let mut candidates = Vec::new();
                            for predicate in predicates.iter() {
                                if let syn::WherePredicate::Type(pred_type) = predicate {
                                    for bound in pred_type.bounds.iter() {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(path) = trait_bound.path.segments.last() {
                                                if path.ident == "eq" || path.ident == "ne" {
                                                    candidates.push(predicate);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if candidates.len() > 0 {
                                let target = candidates.choose(&mut rng).unwrap();
                                let target_idx = predicates.iter().position(|p| p == *target).unwrap();
                                if let syn::WherePredicate::Type(pred_type) = &mut predicates[target_idx] {
                                    for bound in pred_type.bounds.iter_mut() {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(path) = trait_bound.path.segments.last_mut() {
                                                if path.ident == "eq" {
                                                    path.ident = Ident::new("ne", Span::call_site());
                                                } else if path.ident == "ne" {
                                                    path.ident = Ident::new("eq", Span::call_site());
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
        "The mutation operator targets trait method implementations with where clauses involving equality or inequality comparisons between const generic parameters and associated constants. It flips comparison operators (== to !=, != to ==) in trait bounds, altering trait bound satisfaction. This can expose latent deadlocks or ICEs in the compiler's const evaluation and trait resolution, especially under concurrent compilation with generic const expressions."
    }
}