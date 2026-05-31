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

pub struct Replace_Trait_Bounds_333;

impl Mutator for Replace_Trait_Bounds_333 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_333"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                match func.sig.generics {
                    syn::Generics::None(_) => continue,
                    syn::Generics::WhereClause(where_clause) => {
                        for param in where_clause.where_clause.predicates {
                            if let WherePredicate::Type(predicate_type) = param {
                                for bound in &mut predicate_type.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let new_trait_bound = match rng.gen_range(0..3) {
                                            0 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Send", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            1 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Sync", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            2 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Clone", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            _ => unreachable!(),
                                        };
                                        *trait_bound = new_trait_bound;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                match impl_item.generics {
                    syn::Generics::None(_) => continue,
                    syn::Generics::WhereClause(where_clause) => {
                        for param in where_clause.where_clause.predicates {
                            if let WherePredicate::Type(predicate_type) = param {
                                for bound in &mut predicate_type.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let new_trait_bound = match rng.gen_range(0..3) {
                                            0 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Send", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            1 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Sync", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            2 => TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Clone", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            },
                                            _ => unreachable!(),
                                        };
                                        *trait_bound = new_trait_bound;
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
        "The mutation operator replaces trait bounds in a given program with a different trait bound that is compatible with the type. This transformation aims to test the compiler's handling of trait bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}