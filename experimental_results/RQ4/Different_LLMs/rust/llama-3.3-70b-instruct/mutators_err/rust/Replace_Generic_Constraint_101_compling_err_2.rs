use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Generic_Constraint_101;

impl Mutator for Replace_Generic_Constraint_101 {
    fn name(&self) -> &str {
        "Replace_Generic_Constraint_101"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let bounds: Vec<_> = predicate_type.bounds.iter().collect();
                            if !bounds.is_empty() {
                                let new_bound = match bounds[0] {
                                    TypeParamBound::Trait(trait_bound) => {
                                        let new_trait_path = match trait_bound.path.get_ident() {
                                            Some(ident) => {
                                                let new_ident = match ident.to_string().as_str() {
                                                    "Sized" => "Copy",
                                                    "Copy" => "Debug",
                                                    "Debug" => "Sized",
                                                    _ => "Sized",
                                                };
                                                Ident::new(new_ident, ident.span())
                                            }
                                            None => Ident::new("Sized", trait_bound.path.span()),
                                        };
                                        TypeParamBound::Trait(TraitBound {
                                            paren_token: trait_bound.paren_token,
                                            modifier: trait_bound.modifier,
                                            lifetimes: trait_bound.lifetimes.clone(),
                                            path: SynPath {
                                                leading_colon: trait_bound.path.leading_colon,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: new_trait_path,
                                                    arguments: trait_bound.path.segments[0].arguments.clone(),
                                                }]),
                                            },
                                        })
                                    }
                                    _ => unreachable!(),
                                };
                                predicate_type.bounds = Punctuated::from_iter(vec![new_bound]);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    let bounds: Vec<_> = predicate_type.bounds.iter().collect();
                                    if !bounds.is_empty() {
                                        let new_bound = match bounds[0] {
                                            TypeParamBound::Trait(trait_bound) => {
                                                let new_trait_path = match trait_bound.path.get_ident() {
                                                    Some(ident) => {
                                                        let new_ident = match ident.to_string().as_str() {
                                                            "Sized" => "Copy",
                                                            "Copy" => "Debug",
                                                            "Debug" => "Sized",
                                                            _ => "Sized",
                                                        };
                                                        Ident::new(new_ident, ident.span())
                                                    }
                                                    None => Ident::new("Sized", trait_bound.path.span()),
                                                };
                                                TypeParamBound::Trait(TraitBound {
                                                    paren_token: trait_bound.paren_token,
                                                    modifier: trait_bound.modifier,
                                                    lifetimes: trait_bound.lifetimes.clone(),
                                                    path: SynPath {
                                                        leading_colon: trait_bound.path.leading_colon,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: new_trait_path,
                                                            arguments: trait_bound.path.segments[0].arguments.clone(),
                                                        }]),
                                                    },
                                                })
                                            }
                                            _ => unreachable!(),
                                        };
                                        predicate_type.bounds = Punctuated::from_iter(vec![new_bound]);
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
        "The mutation operator replaces generic constraints in function and method signatures. It swaps the trait bounds of the first constraint with a similar but not identical one, aiming to test the compiler's handling of generic constraints and trait resolution."
    }
}