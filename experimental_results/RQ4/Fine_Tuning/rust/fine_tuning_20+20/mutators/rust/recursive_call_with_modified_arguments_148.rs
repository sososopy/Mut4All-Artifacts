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

pub struct Recursive_Call_With_Modified_Arguments_148;

impl Mutator for Recursive_Call_With_Modified_Arguments_148 {
    fn name(&self) -> &str {
        "Recursive_Call_With_Modified_Arguments_148"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    let mut has_recursive_call = false;
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                if path.is_ident(&item_fn.sig.ident) {
                                    has_recursive_call = true;
                                    break;
                                }
                            }
                        }
                    }
                    if !has_recursive_call {
                        let new_call: Expr = parse_quote! { main() };
                        item_fn.block.stmts.push(Stmt::Expr(new_call, None));
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    let mut arr_declared = false;
                    let mut i_declared = false;
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Local(Local { pat, init, .. }) = stmt {
                            if let Pat::Ident(pat_ident) = pat {
                                if pat_ident.ident == "arr" {
                                    arr_declared = true;
                                }
                                if pat_ident.ident == "i" {
                                    i_declared = true;
                                }
                            }
                            if let Some(init) = init {
                                if let Expr::Array(_) = &*init.expr {
                                    arr_declared = true;
                                }
                            }
                        }
                    }
                    if !arr_declared {
                        let new_stmt: Stmt = parse_quote! {
                            let arr = vec![1, 2, 3];
                        };
                        item_fn.block.stmts.push(new_stmt);
                    }
                    if !i_declared {
                        let new_stmt: Stmt = parse_quote! {
                            let i = 10;
                        };
                        item_fn.block.stmts.push(new_stmt);
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                if path.is_ident(&item_fn.sig.ident) {
                                    expr_call.args = parse_quote! { arr[i] };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets the main function by ensuring it contains a recursive call with modified arguments. It introduces or modifies a recursive call to use an array or vector with an index that may be undefined or out-of-bounds, leveraging patterns known to trigger assertion failures in the Rust compiler's handling of indexing and recursion."
    }
}