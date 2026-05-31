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

pub struct Missing_Lifetime_In_Trait_Implementation_193;

impl Mutator for Missing_Lifetime_In_Trait_Implementation_193 {
    fn name(&self) -> &str {
        "Missing_Lifetime_In_Trait_Implementation_193"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        let mut trait_lifetimes = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let has_async_fn = item_trait.items.iter().any(|item| match item {
                    TraitItem::Fn(trait_fn) => trait_fn.sig.asyncness.is_some(),
                    _ => false,
                });
                if has_async_fn {
                    trait_names.insert(item_trait.ident.to_string());
                    for param in &item_trait.generics.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            trait_lifetimes.insert(lifetime.lifetime.ident.to_string());
                        }
                    }
                }
            }
        }
        let mut struct_names = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                struct_names.insert(item_struct.ident.to_string());
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &mut item_impl.trait_ {
                    let trait_name = path
                        .segments
                        .last()
                        .map(|segment| segment.ident.to_string());
                    if let Some(trait_name) = trait_name {
                        if trait_names.contains(&trait_name) {
                            path.segments = path
                                .segments
                                .iter()
                                .filter(|segment| !trait_lifetimes.contains(&segment.ident.to_string()))
                                .cloned()
                                .collect();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations for traits with async functions and lifetime parameters. It identifies such traits and their associated lifetimes, then modifies implementations by removing the lifetime parameters from the trait path in the impl block. This transformation exploits a specific compiler weakness related to lifetime elision and async trait handling, potentially leading to ICEs when the compiler attempts to resolve the missing lifetimes."
    }
}