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

pub struct Replace_Lifetime_Bound_With_Self_Referential_158;

impl Mutator for Replace_Lifetime_Bound_With_Self_Referential_158 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Bound_With_Self_Referential_158"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let sig = &mut item_fn.sig;
                if let ReturnType::Type(_, return_type) = &sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let lifetime_params = sig.generics.params.iter().filter_map(|param| {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                Some(lifetime_param.lifetime.clone())
                            } else {
                                None
                            }
                        }).collect::<Vec<_>>();
                        if !lifetime_params.is_empty() {
                            let mut where_clause = sig.generics.where_clause.take();
                            if where_clause.is_none() {
                                where_clause = Some(syn::WhereClause {
                                    where_token: token::Where::default(),
                                    predicates: Punctuated::new(),
                                });
                            }
                            let mut predicates = where_clause.as_mut().unwrap().predicates.clone();
                            for lifetime in lifetime_params {
                                let self_bound = syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                    lifetime: lifetime.clone(),
                                    colon_token: token::Colon::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(Lifetime::new(&lifetime.ident.to_string(), lifetime.span()));
                                        bounds
                                    },
                                });
                                let existing_predicate = predicates.iter_mut().find(|pred| {
                                    if let syn::WherePredicate::Lifetime(pred_lifetime) = pred {
                                        pred_lifetime.lifetime == lifetime
                                    } else {
                                        false
                                    }
                                });
                                if let Some(existing_predicate) = existing_predicate {
                                    if let syn::WherePredicate::Lifetime(pred_lifetime) = existing_predicate {
                                        pred_lifetime.bounds.clear();
                                        pred_lifetime.bounds.push(Lifetime::new(&lifetime.ident.to_string(), lifetime.span()));
                                    }
                                } else {
                                    predicates.push(self_bound);
                                }
                            }
                            where_clause.as_mut().unwrap().predicates = predicates;
                            sig.generics.where_clause = where_clause;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return `impl Trait` and have lifetime parameters. It modifies the where clause to introduce self-referential lifetime bounds (`'a: 'a`), replacing existing bounds or adding new ones. This transformation creates tautological constraints that can expose bugs in region inference and trait resolution for opaque return types, maximizing coverage by affecting common Rust patterns involving generics and lifetimes."
    }
}