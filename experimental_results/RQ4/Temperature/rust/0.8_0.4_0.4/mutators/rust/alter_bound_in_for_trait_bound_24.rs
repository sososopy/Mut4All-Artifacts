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

pub struct Alter_Bound_In_For_Trait_Bound_24;

impl Mutator for Alter_Bound_In_For_Trait_Bound_24 {
    fn name(&self) -> &str {
        "Alter_Bound_In_For_Trait_Bound_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Vec::new();
                            for bound in &predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.lifetimes.is_some() {
                                        let new_lifetime: Lifetime = parse_quote!('b);
                                        let new_bound = syn::TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: Some(BoundLifetimes {
                                                for_token: Default::default(),
                                                lt_token: Default::default(),
                                                lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                                    attrs: Vec::new(),
                                                    lifetime: new_lifetime.clone(),
                                                    colon_token: None,
                                                    bounds: Punctuated::new(),
                                                })]),
                                                gt_token: Default::default(),
                                            }),
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segs = Punctuated::new();
                                                    segs.push(syn::PathSegment {
                                                        ident: Ident::new("F", Span::call_site()),
                                                        arguments: syn::PathArguments::None,
                                                    });
                                                    segs
                                                },
                                            },
                                        });
                                        new_bounds.push(new_bound);
                                    }
                                }
                            }
                            predicate_type.bounds.extend(new_bounds);
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