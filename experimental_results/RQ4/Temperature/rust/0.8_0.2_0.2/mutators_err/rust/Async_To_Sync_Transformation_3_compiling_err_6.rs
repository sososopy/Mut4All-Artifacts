use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprAwait, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Async_To_Sync_Transformation_3;

impl Mutator for Async_To_Sync_Transformation_3 {
    fn name(&self) -> &str {
        "Async_To_Sync_Transformation_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) | Stmt::Semi(expr, _) = stmt {
                            remove_await(expr);
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                            for stmt in &mut func.block.stmts {
                                if let Stmt::Expr(expr, _) | Stmt::Semi(expr, _) = stmt {
                                    remove_await(expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms async functions and blocks into synchronous ones by removing the `async` keyword and `await` operators. This tests the program's behavior when asynchronous constructs are converted to synchronous execution, potentially revealing issues related to concurrency management and async/await handling in the Rust compiler."
    }
}

fn remove_await(expr: &mut Expr) {
    match expr {
        Expr::Await(expr_await) => {
            *expr = *expr_await.base.clone();
        }
        Expr::Block(expr_block) => {
            for stmt in &mut expr_block.block.stmts {
                if let Stmt::Expr(inner_expr, _) | Stmt::Semi(inner_expr, _) = stmt {
                    remove_await(inner_expr);
                }
            }
        }
        _ => {}
    }
}