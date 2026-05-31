use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Trait_Bounds_411;

impl Mutator for Replace_Trait_Bounds_411 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_411"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut new_bounds = Punctuated::new();
                let mut rng = thread_rng();

                // Randomly decide whether to add, remove or modify bounds
                let action: u8 = rng.gen_range(0..3);

                match action {
                    0 => {
                        // Add new bounds
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: Ident::new("Send", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }));
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: Ident::new("Sync", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }));
                    }
                    1 => {
                        // Remove existing bounds
                        if let Some(where_clause) = &trait_item.generics.where_clause {
                            for predicate in where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Some(bounds) = predicate_type.bounds.as_ref() {
                                        for bound in bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path.segments.last().unwrap().ident == "Sized" {
                                                    // Remove the 'Sized' bound
                                                    continue;
                                                } else {
                                                    new_bounds.push(bound.clone());
                                                }
                                            } else {
                                                new_bounds.push(bound.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    2 => {
                        // Modify existing bounds
                        if let Some(where_clause) = &trait_item.generics.where_clause {
                            for predicate in where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Some(bounds) = predicate_type.bounds.as_ref() {
                                        for bound in bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                // Replace the bound with a new one
                                                new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                                    paren_token: None,
                                                    modifier: syn::TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: syn::Path {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                            ident: Ident::new("Copy", Span::call_site()),
                                                            arguments: syn::PathArguments::None,
                                                        }]),
                                                    },
                                                }));
                                            } else {
                                                new_bounds.push(bound.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    _ => unreachable!(),
                }

                // Update the trait bounds
                if let Some(where_clause) = &mut trait_item.generics.where_clause {
                    where_clause.predicates.clear();
                    where_clause.predicates.push(syn::WherePredicate::Type(syn::PredicateType {
                        lifetimes: None,
                        bounded_ty: syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: Ident::new("Self", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }),
                        colon_token: Default::default(),
                        bounds: new_bounds,
                    }));
                } else {
                    trait_item.generics.where_clause = Some(syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::from_iter(vec![syn::WherePredicate::Type(syn::PredicateType {
                            lifetimes: None,
                            bounded_ty: syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                                        ident: Ident::new("Self", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    }]),
                                },
                            }),
                            colon_token: Default::default(),
                            bounds: new_bounds,
                        })]),
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Replace_Trait_Bounds_411 mutation operator randomly adds, removes or modifies trait bounds in trait definitions. This transformation tests the compiler's ability to handle different types of trait bounds and ensures that it correctly enforces these bounds when checking the validity of a program."
    }
}