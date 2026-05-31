use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Change_Trait_Bound_272;

impl Mutator for Change_Trait_Bound_272 {
    fn name(&self) -> &str {
        "Change_Trait_Bound_272"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::Generics { where_clause, .. } = &func.sig.generics {
                    if let Some(clauses) = where_clause {
                        for param in &mut clauses.predicates {
                            if let syn::WherePredicate::Type(predicate) = param {
                                if let Some(bounds) = &predicate.bounds {
                                    for bound in bounds.iter_mut() {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            let new_trait_bound = TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Service", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            };
                                            *bound = TypeParamBound::Trait(new_trait_bound);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let syn::Generics { where_clause, .. } = &impl_item.generics {
                    if let Some(clauses) = where_clause {
                        for param in &mut clauses.predicates {
                            if let syn::WherePredicate::Type(predicate) = param {
                                if let Some(bounds) = &predicate.bounds {
                                    for bound in bounds.iter_mut() {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            let new_trait_bound = TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new("Service", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            };
                                            *bound = TypeParamBound::Trait(new_trait_bound);
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
        "The Change_Trait_Bound_272 mutation operator targets the trait bounds of a type parameter in a generic type or function. It applies to any code that uses generics with trait bounds. This operator changes the trait bound of a type parameter by replacing it with a different trait or changing its associated types. For example, in the given code, it could change `F: Framing` to `F: Service` or `F: Framing + Service`. This mutation can help test how the compiler handles changes in trait bounds and associated types."
    }
}