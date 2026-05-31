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

pub struct Replace_Async_With_Sync_249;

impl Mutator for Replace_Async_With_Sync_249 {
    fn name(&self) -> &str {
        "Replace_Async_With_Sync_249"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                        }
                    }
                }
            }
            if let syn::Item::Expr(Expr::Async(ExprAsync { expr, .. })) = item {
                *item = syn::Item::Expr(expr.clone());
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions and closures with their synchronous counterparts, effectively removing the async keyword. This transformation aims to test the compiler's handling of async and sync code, potentially leading to bugs related to incorrect usage of await or async move, or issues with coroutine layout and promotion."
    }
}