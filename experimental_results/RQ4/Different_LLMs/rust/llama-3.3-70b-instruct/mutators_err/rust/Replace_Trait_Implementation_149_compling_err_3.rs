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

pub struct Replace_Trait_Implementation_149;

impl Mutator for Replace_Trait_Implementation_149 {
    fn name(&self) -> &str {
        "Replace_Trait_Implementation_149"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls: Vec<(&syn::ItemImpl, &syn::Type)> = Vec::new();

        for item in &file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = get_trait_path(impl_item) {
                    trait_impls.push((impl_item, path));
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = get_trait_path(impl_item) {
                    let mut alternative_impls: Vec<(&syn::ItemImpl, &syn::Type)> = Vec::new();

                    for (alt_impl, alt_path) in &trait_impls {
                        if **alt_path == **path && **alt_impl != **impl_item {
                            alternative_impls.push((alt_impl, alt_path));
                        }
                    }

                    if !alternative_impls.is_empty() {
                        let (alt_impl, _) = alternative_impls.choose(&mut thread_rng()).unwrap();
                        replace_impl(impl_item, alt_impl);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the implementation of a trait for a given type with the implementation of the same trait for a different type. This transformation tests the compiler's ability to handle different trait implementations and ensures that it can correctly handle changes in the code."
    }
}

fn get_trait_path(impl_item: &syn::ItemImpl) -> Option<(&syn::Path, &syn::Type, &syn::PathArguments)> {
    if let Some(trait_path) = &impl_item.trait_ {
        let path = &trait_path.1;
        let args = &path.segments.last().unwrap().arguments;
        Some((path, &impl_item.self_ty, args))
    } else {
        None
    }
}

fn replace_impl(impl_item: &mut syn::ItemImpl, alt_impl: &syn::ItemImpl) {
    impl_item.items = alt_impl.items.clone();
}