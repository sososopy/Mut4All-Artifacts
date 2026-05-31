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

pub struct Introduce_Complex_Generic_Const_Expressions_349;

impl Mutator for Introduce_Complex_Generic_Const_Expressions_349 {
    fn name(&self) -> &str {
        "Introduce_Complex_Generic_Const_Expressions_349"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_generic_const = false;
                for param in &item_fn.sig.generics.params {
                    if matches!(param, GenericParam::Const(_)) {
                        has_generic_const = true;
                        break;
                    }
                }
                let mut has_complex_const_expr = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(Local {
                        init: Some(init_expr),
                        ..
                    }) = stmt
                    {
                        if let Expr::Array(syn::ExprArray { elems, .. }) = &*init_expr.expr {
                            if let Expr::Block(syn::ExprBlock { block, .. }) = &*elems.first().unwrap() {
                                if !block.stmts.is_empty() {
                                    has_complex_const_expr = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_complex_const_expr {
                    if !has_generic_const {
                        item_fn.sig.generics.params.push(parse_quote!(const N: usize));
                    }
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(Local {
                            init: Some(init_expr),
                            ..
                        }) = stmt
                        {
                            if let Expr::Array(syn::ExprArray { elems, .. }) = &mut *init_expr.expr {
                                if let Expr::Block(syn::ExprBlock { block, .. }) = &mut *elems.first_mut().unwrap() {
                                    if block.stmts.len() == 1 {
                                        if let Stmt::Expr(expr, _) = &block.stmts[0] {
                                            if let Expr::Binary(expr_binary) = expr {
                                                if expr_binary.op == syn::BinOp::Add(syn::token::And {
                                                    spans: [..],
                                                }) {
                                                    let new_expr: Expr = parse_quote!(N + 4);
                                                    block.stmts[0] = Stmt::Expr(new_expr, None);
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
        "This mutator targets functions with constant expressions in array initializations, replacing simple expressions like `1 + 4` with more complex generic const expressions involving a type-level parameter `N`, such as `N + 4`. If the function lacks a const generic parameter, it introduces one, transforming the function to leverage Rust's const generics for more intricate compile-time evaluation, thereby increasing the likelihood of exposing compiler bugs related to generic const handling."
    }
}