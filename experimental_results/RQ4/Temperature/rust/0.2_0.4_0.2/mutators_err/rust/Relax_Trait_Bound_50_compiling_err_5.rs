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

pub struct Relax_Trait_Bound_50;

impl Mutator for Relax_Trait_Bound_50 {
    fn name(&self) -> &str {
        "Relax_Trait_Bound_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates = where_clause.predicates.iter().cloned().filter_map(|predicate| {
                        if let syn::WherePredicate::Type(mut predicate_type) = predicate {
                            predicate_type.bounds = predicate_type.bounds.into_iter().filter(|bound| {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &trait_bound.lifetimes {
                                        if lifetimes.lifetimes.iter().any(|lt| lt.ident == "'a") {
                                            return false;
                                        }
                                    }
                                }
                                true
                            }).collect();
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.lifetimes.is_some() {
                                        trait_bound.lifetimes = None;
                                    }
                                }
                            }
                            Some(syn::WherePredicate::Type(predicate_type))
                        } else {
                            Some(predicate)
                        }
                    }).collect();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}