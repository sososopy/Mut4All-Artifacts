use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct ReplaceAsyncWithSync_19;

impl Mutator for ReplaceAsyncWithSync_19 {
    fn name(&self) -> &str {
        "ReplaceAsyncWithSync_19"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr) = stmt {
                            if let Expr::Await(await_expr) = expr {
                                *expr = *await_expr.base.clone();
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                            for stmt in &mut func.block.stmts {
                                if let Stmt::Expr(expr) = stmt {
                                    if let Expr::Await(await_expr) = expr {
                                        *expr = *await_expr.base.clone();
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
        "The mutation operator replaces async functions with their synchronous counterparts by removing the async keyword. It also updates await expressions within the function body to ensure they are properly handled in the synchronous context. This transformation aims to test the compiler's handling of asynchronous programming and may lead to bugs related to incorrect usage of async/await, deadlocks, or improper synchronization."
    }
}