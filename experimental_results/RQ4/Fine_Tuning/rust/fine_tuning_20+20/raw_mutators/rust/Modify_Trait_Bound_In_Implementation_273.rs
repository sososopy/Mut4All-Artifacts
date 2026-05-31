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

pub struct Modify_Trait_Bound_In_Implementation_273;

impl Mutator for Modify_Trait_Bound_In_Implementation_273 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Implementation_273"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if let Some(where_clause) = &mut item_impl.generics.where_clause {
                        let mut rng = thread_rng();
                        let mut trait_bounds = Vec::new();
                        for (i, predicate) in where_clause.predicates.iter().enumerate() {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                for (j, bound) in predicate_type.bounds.iter().enumerate() {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bounds.push((i, j, trait_bound.clone()));
                                    }
                                }
                            }
                        }
                        if let Some(&(i, j, ref trait_bound)) = trait_bounds.choose(&mut rng) {
                            let mut candidates = Vec::new();
                            for (k, predicate) in where_clause.predicates.iter().enumerate() {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    if k == i {
                                        for (l, bound) in
                                            predicate_type.bounds.iter().enumerate()
                                        {
                                            if l != j {
                                                candidates.push(bound.clone());
                                            }
                                        }
                                    } else {
                                        for bound in &predicate_type.bounds {
                                            candidates.push(bound.clone());
                                        }
                                    }
                                }
                            }
                            if let Some(new_bound) = candidates.choose(&mut rng) {
                                let new_bound = new_bound.clone();
                                let mut new_predicates = Punctuated::new();
                                for (k, predicate) in
                                    where_clause.predicates.iter().enumerate()
                                {
                                    if let syn::WherePredicate::Type(predicate_type) =
                                        predicate
                                    {
                                        if k == i {
                                            let new_predicate_type =
                                                syn::WherePredicate::Type(
                                                    syn::PredicateType {
                                                        lifetimes: predicate_type
                                                            .lifetimes
                                                            .clone(),
                                                        bounded_ty: predicate_type
                                                            .bounded_ty
                                                            .clone(),
                                                        colon_token: predicate_type
                                                            .colon_token,
                                                        bounds: {
                                                            let mut new_bounds =
                                                                Punctuated::new();
                                                            for (l, bound) in
                                                                predicate_type
                                                                    .bounds
                                                                    .iter()
                                                                    .enumerate()
                                                            {
                                                                if l == j {
                                                                    new_bounds.push(
                                                                        new_bound
                                                                            .clone(),
                                                                    );
                                                                } else {
                                                                    new_bounds.push(
                                                                        bound.clone(),
                                                                    );
                                                                }
                                                            }
                                                            new_bounds
                                                        },
                                                    },
                                                );
                                            new_predicates
                                                .push(syn::WherePredicate::Type(
                                                    new_predicate_type,
                                                ));
                                        } else {
                                            new_predicates.push(predicate.clone());
                                        }
                                    }
                                }
                                where_clause.predicates = new_predicates;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets trait implementations with where clauses, specifically altering one of the trait bounds in the where clause to a different bound present in the same or other predicates. This transformation aims to introduce subtle type mismatches in trait constraints, potentially leading to unexpected behavior in trait resolution and associated type inference, thereby exposing weaknesses in the compiler's handling of trait bounds."
    }
}