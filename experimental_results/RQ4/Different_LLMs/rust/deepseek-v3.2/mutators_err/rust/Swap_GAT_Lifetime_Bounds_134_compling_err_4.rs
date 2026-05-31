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

pub struct Swap_GAT_Lifetime_Bounds_134;

impl Mutator for Swap_GAT_Lifetime_Bounds_134 {
    fn name(&self) -> &str {
        "Swap_GAT_Lifetime_Bounds_134"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_gats = Vec::new();
        let mut impl_gats = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Type(type_item) = trait_item {
                        trait_gats.push((trait_item, type_item));
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        impl_gats.push((impl_item, type_item));
                    }
                }
            }
        }
        for (trait_item, gat) in trait_gats {
            let mut lifetimes_in_scope = HashSet::new();
            for param in &gat.generics.params {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    lifetimes_in_scope.insert(lifetime_param.lifetime.clone());
                }
            }
            for param in &trait_item.generics.params {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    lifetimes_in_scope.insert(lifetime_param.lifetime.clone());
                }
            }
            if lifetimes_in_scope.len() >= 2 {
                let mut rng = thread_rng();
                let lifetimes_vec: Vec<Lifetime> = lifetimes_in_scope.into_iter().collect();
                let lifetime_a = lifetimes_vec.choose(&mut rng).unwrap();
                let lifetime_b = lifetimes_vec.choose(&mut rng).unwrap();
                let new_predicate = WherePredicate::Lifetime(PredicateLifetime {
                    lifetime: lifetime_a.clone(),
                    colon_token: Default::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_b.clone());
                        bounds
                    },
                });
                if !gat.bounds.is_empty() {
                    let mut new_predicates = gat.bounds.clone();
                    new_predicates.push(TypeParamBound::Lifetime(lifetime_b.clone()));
                    gat.bounds = new_predicates;
                } else {
                    let mut bounds = Punctuated::new();
                    bounds.push(TypeParamBound::Lifetime(lifetime_b.clone()));
                    gat.bounds = bounds;
                }
            }
        }
        for (impl_item, gat) in impl_gats {
            let mut lifetimes_in_scope = HashSet::new();
            for param in &gat.generics.params {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    lifetimes_in_scope.insert(lifetime_param.lifetime.clone());
                }
            }
            for param in &impl_item.generics.params {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    lifetimes_in_scope.insert(lifetime_param.lifetime.clone());
                }
            }
            if lifetimes_in_scope.len() >= 2 {
                let mut rng = thread_rng();
                let lifetimes_vec: Vec<Lifetime> = lifetimes_in_scope.into_iter().collect();
                let lifetime_a = lifetimes_vec.choose(&mut rng).unwrap();
                let lifetime_b = lifetimes_vec.choose(&mut rng).unwrap();
                let new_predicate = WherePredicate::Lifetime(PredicateLifetime {
                    lifetime: lifetime_a.clone(),
                    colon_token: Default::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_b.clone());
                        bounds
                    },
                });
                if !gat.bounds.is_empty() {
                    let mut new_predicates = gat.bounds.clone();
                    new_predicates.push(TypeParamBound::Lifetime(lifetime_b.clone()));
                    gat.bounds = new_predicates;
                } else {
                    let mut bounds = Punctuated::new();
                    bounds.push(TypeParamBound::Lifetime(lifetime_b.clone()));
                    gat.bounds = bounds;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic associated type (GAT) declarations in trait definitions and implementations. It modifies the where clause constraints by swapping or altering lifetime relationships. For GATs with existing where clauses, it adds a new lifetime predicate that swaps the direction of a bound or uses unrelated lifetimes from the current scope. If no where clause exists, one is created with a mutated constraint. This transformation aims to expose bugs in the compiler's handling of complex lifetime bounds in GATs, particularly around higher-ranked trait bounds and trait solving, which can lead to internal compiler errors."
    }
}