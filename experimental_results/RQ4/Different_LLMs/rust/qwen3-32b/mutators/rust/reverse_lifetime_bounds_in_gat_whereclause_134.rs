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

pub struct Reverse_Lifetime_Bounds_In_GAT_WhereClause_134;

impl Mutator for Reverse_Lifetime_Bounds_In_GAT_WhereClause_134 {
    fn name(&self) -> &str {
        "Reverse_Lifetime_Bounds_In_GAT_WhereClause_134"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(assoc_type) = trait_item {
                        if let Some(where_clause) = &mut assoc_type.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Lifetime(lifetime_def) = predicate {
                                    if let Some(first) = lifetime_def.bounds.iter().next() {
                                        let new_lifetimes = syn::punctuated::Punctuated::from_iter(vec![lifetime_def.lifetime.clone()]);
                                        let new_def = syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                            lifetime: first.clone(),
                                            colon_token: lifetime_def.colon_token,
                                            bounds: new_lifetimes,
                                        });
                                        *predicate = new_def;
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(assoc_type) = impl_item {
                        if let Some(where_clause) = &mut assoc_type.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Lifetime(lifetime_def) = predicate {
                                    if let Some(first) = lifetime_def.bounds.iter().next() {
                                        let new_lifetimes = syn::punctuated::Punctuated::from_iter(vec![lifetime_def.lifetime.clone()]);
                                        let new_def = syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                            lifetime: first.clone(),
                                            colon_token: lifetime_def.colon_token,
                                            bounds: new_lifetimes,
                                        });
                                        *predicate = new_def;
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
        ""
    }
}