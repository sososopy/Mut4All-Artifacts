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

pub struct Remove_Trait_Implementation_457;

impl Mutator for Remove_Trait_Implementation_457 {
    fn name(&self) -> &str {
        "Remove_Trait_Implementation_457"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = get_trait_path(item_impl) {
                    trait_impls.push(trait_path.clone());
                }
            }
        }
        if let Some(trait_impl) = trait_impls.choose(&mut thread_rng()) {
            file.items.retain(|item| {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = get_trait_path(item_impl) {
                        return *path != *trait_impl;
                    }
                }
                true
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes a random trait implementation from the code. This transformation tests the compiler's ability to handle missing trait implementations and may lead to errors or warnings related to trait resolution."
    }
}

fn get_trait_path(item_impl: &syn::ItemImpl) -> Option<(&syn::Type, &syn::Path, Option<&syn::WhereClause>)> {
    if let Some(trait_path) = &item_impl.trait_ {
        let ty = &item_impl.self_ty;
        let where_clause = &item_impl.generics.where_clause;
        Some((ty, &trait_path.1, where_clause.as_ref()))
    } else {
        None
    }
}