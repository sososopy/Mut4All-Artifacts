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

pub struct Replace_Trait_Implementation_449;

impl Mutator for Replace_Trait_Implementation_449 {
    fn name(&self) -> &str {
        "Replace_Trait_Implementation_449"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(trait_path) = &item_impl.trait_ {
                    let trait_path = trait_path.1.clone();
                    let default_impl = find_default_implementation(file, &trait_path);
                    if let Some(default_impl) = default_impl {
                        replace_specialized_implementation(item_impl, default_impl);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the implementation of traits in Rust code, specifically focusing on the specialization feature. It replaces the specialized implementation with the default implementation, effectively removing the specialization. This change can help expose bugs related to trait specialization, such as incorrect handling of overlapping implementations or issues with the coherence of trait implementations."
    }
}

fn find_default_implementation(file: &syn::File, trait_path: &syn::Path) -> Option<syn::ItemImpl> {
    for item in &file.items {
        if let syn::Item::Impl(item_impl) = item {
            if let Some(trait_path_impl) = &item_impl.trait_ {
                if trait_path_impl.1 == *trait_path {
                    if item_impl.defaultness.is_some() {
                        return Some(item_impl.clone());
                    }
                }
            }
        }
    }
    None
}

fn replace_specialized_implementation(item_impl: &mut syn::ItemImpl, default_impl: syn::ItemImpl) {
    for impl_item in &mut item_impl.items {
        if let syn::ImplItem::Fn(func) = impl_item {
            for default_impl_item in &default_impl.items {
                if let syn::ImplItem::Fn(default_func) = default_impl_item {
                    if func.sig.ident == default_func.sig.ident {
                        *func = default_func.clone();
                    }
                }
            }
        }
    }
}