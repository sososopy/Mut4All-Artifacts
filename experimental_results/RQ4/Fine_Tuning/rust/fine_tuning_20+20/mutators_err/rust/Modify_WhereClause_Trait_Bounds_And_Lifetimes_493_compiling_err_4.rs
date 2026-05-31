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

pub struct Modify_WhereClause_Trait_Bounds_And_Lifetimes_493;

fn find_function_trait_bound_index(
    bounds: &Punctuated<TypeParamBound, token::Plus>,
) -> Option<usize> {
    bounds.iter().position(|bound| {
        if let TypeParamBound::Trait(trait_bound) = bound {
            let path = &trait_bound.path;
            path.segments.iter().any(|seg| {
                let ident = &seg.ident;
                ident == "Fn" || ident == "FnMut" || ident == "FnOnce"
            })
        } else {
            false
        }
    })
}

fn find_lifetime_in_trait_bound(trait_bound: &TraitBound) -> Option<Lifetime> {
    trait_bound.lifetimes.as_ref().and_then(|bound_lifetimes| {
        bound_lifetimes
            .lifetimes
            .first()
            .map(|lifetime| lifetime.clone())
    })
}

fn replace_function_trait_bound(trait_bound: &mut TraitBound) {
    let mut rng = thread_rng();
    let new_trait_ident = match rng.gen_range(0..3) {
        0 => Ident::new("Fn", Span::call_site()),
        1 => Ident::new("FnMut", Span::call_site()),
        _ => Ident::new("FnOnce", Span::call_site()),
    };
    let new_path = SynPath {
        leading_colon: trait_bound.path.leading_colon,
        segments: {
            let mut segments = Punctuated::new();
            segments.push(PathSegment {
                ident: new_trait_ident,
                arguments: PathArguments::None,
            });
            segments
        },
    };
    trait_bound.path = new_path;
}

fn modify_lifetime_in_trait_bound(trait_bound: &mut TraitBound) {
    if let Some(bound_lifetimes) = &mut trait_bound.lifetimes {
        let mut rng = thread_rng();
        let new_lifetime_ident = match rng.gen_range(0..3) {
            0 => Ident::new("a", Span::call_site()),
            1 => Ident::new("b", Span::call_site()),
            _ => Ident::new("c", Span::call_site()),
        };
        bound_lifetimes.lifetimes = {
            let mut lifetimes = Punctuated::new();
            lifetimes.push(Lifetime {
                apostrophe: Span::call_site(),
                ident: new_lifetime_ident,
            });
            lifetimes
        };
    }
}

fn modify_lifetime_in_predicate_type(predicate_type: &mut PredicateType) {
    let mut rng = thread_rng();
    let new_lifetime_ident = match rng.gen_range(0..3) {
        0 => Ident::new("a", Span::call_site()),
        1 => Ident::new("b", Span::call_site()),
        _ => Ident::new("c", Span::call_site()),
    };
    predicate_type.lifetimes = Some(BoundLifetimes {
        for_token: token::For {
            span: Span::call_site(),
        },
        lt_token: token::Lt {
            spans: [Span::call_site()],
        },
        lifetimes: {
            let mut lifetimes = Punctuated::new();
            lifetimes.push(Lifetime {
                apostrophe: Span::call_site(),
                ident: new_lifetime_ident,
            });
            lifetimes
        },
        gt_token: token::Gt {
            spans: [Span::call_site()],
        },
    });
}

impl Mutator for Modify_WhereClause_Trait_Bounds_And_Lifetimes_493 {
    fn name(&self) -> &str {
        "Modify_WhereClause_Trait_Bounds_And_Lifetimes_493"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            let bounds = &mut predicate_type.bounds;
                            if let Some(index) =
                                find_function_trait_bound_index(bounds)
                            {
                                if let Some(TypeParamBound::Trait(trait_bound)) =
                                    bounds.get(index)
                                {
                                    if let Some(lifetime) =
                                        find_lifetime_in_trait_bound(trait_bound)
                                    {
                                        modify_lifetime_in_predicate_type(
                                            predicate_type,
                                        );
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
        "The mutator targets function where clauses with complex trait bounds and lifetimes, specifically altering function trait bounds and associated lifetimes to stress-test the compiler's handling of trait resolution and lifetime inference."
    }
}