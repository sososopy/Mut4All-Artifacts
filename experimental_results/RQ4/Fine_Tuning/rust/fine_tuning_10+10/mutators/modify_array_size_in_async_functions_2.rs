use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Modify_Array_Size_In_Async_Functions_2;

impl Mutator for Modify_Array_Size_In_Async_Functions_2 {
    fn name(&self) -> &str {
        "Modify_Array_Size_In_Async_Functions_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Array(expr_array) = &*init.expr {
                                    if let Some(expr) = expr_array.elems.first() {
                                        if let Expr::Lit(expr_lit) = expr {
                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                if lit_int.suffix() == "usize" {
                                                    let new_expr: Expr = parse_quote!([1usize; usize::MAX]);
                                                    *stmt = Stmt::Local(Local {
                                                        init: Some(LocalInit {
                                                            eq_token: init.eq_token.clone(),
                                                            expr: Box::new(new_expr),
                                                            diverge: None,
                                                        }),
                                                        ..local.clone()
                                                    });
                                                }
                                            }
                                        }
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
        "This mutation operator targets array declarations within async functions. It identifies arrays of usize type and modifies their size to usize::MAX. This change is applied only within async functions to potentially expose issues related to async-await handling in Rust."
    }
}