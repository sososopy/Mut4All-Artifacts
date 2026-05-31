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

pub struct Replace_Async_With_Sync_169;

impl Mutator for Replace_Async_With_Sync_169 {
    fn name(&self) -> &str {
        "Replace_Async_With_Sync_169"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Await(await_expr) = **expr {
                                *expr = *await_expr.base;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async function calls with equivalent sync function calls by removing the async keyword and the await expression. This transformation tests how the compiler handles the change from asynchronous to synchronous code, which can help expose bugs related to concurrency and async/await functionality."
    }
}