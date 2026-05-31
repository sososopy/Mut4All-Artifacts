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

pub struct Replace_Trait_Bounds_355;

impl Mutator for Replace_Trait_Bounds_355 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_355"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.supertraits {
                    if let Type::Path(type_path) = item {
                        let new_trait_bound = TraitBound {
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
                        };
                        *item = Type::TraitObject(TypeTraitObject {
                            dyn_token: token::Dyn::default(),
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]),
                        });
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    let new_trait_bound = TraitBound {
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
                    };
                    impl_item.trait_ = Some((None, SynPath {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![PathSegment {
                            ident: Ident::new("Send", Span::call_site()),
                            arguments: PathArguments::None,
                        }]),
                    }, None));
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_trait_bound = TraitBound {
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds in trait definitions, trait implementations, and function signatures with a new trait bound. This transformation aims to test the compiler's handling of trait bounds and generics, and to exercise the interactions between different traits and types."
    }
}