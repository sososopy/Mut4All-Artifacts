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

pub struct Misplace_Await_In_Async_Function_169;

impl Mutator for Misplace_Await_In_Async_Function_169 {
    fn name(&self) -> &str {
        "Misplace_Await_In_Async_Function_169"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let mut misplaced_await = None;
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(Expr::Call(call), _) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &*call.func {
                                if path.segments.last().unwrap().ident == "await" {
                                    misplaced_await = Some(call.clone());
                                    break;
                                }
                            }
                        }
                    }
                    if let Some(mut call) = misplaced_await {
                        if let Expr::Path(ExprPath { path, .. }) = &*call.func {
                            if path.segments.last().unwrap().ident == "await" {
                                call.func = Box::new(parse_quote!(initial_exchange));
                                func.block.stmts.push(Stmt::Expr(Expr::Call(call), None));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions, removing the `.await` from an async function call and placing it after a non-async expression. This misplacement aims to test the compiler's handling of incorrect await usage, potentially causing runtime errors or unexpected behavior in async code."
    }
}