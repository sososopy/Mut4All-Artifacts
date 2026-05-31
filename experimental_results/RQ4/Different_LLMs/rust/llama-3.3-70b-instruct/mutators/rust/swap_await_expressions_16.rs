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

pub struct Swap_Await_Expressions_16;

impl Mutator for Swap_Await_Expressions_16 {
    fn name(&self) -> &str {
        "Swap_Await_Expressions_16"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut await_exprs: Vec<Stmt> = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Await(await_expr) = expr {
                            await_exprs.push(stmt.clone());
                        }
                    }
                }
            }
        }

        if await_exprs.len() >= 2 {
            let mut rng = thread_rng();
            let first_idx = rng.gen_range(0..await_exprs.len());
            let second_idx = rng.gen_range(0..await_exprs.len());
            if first_idx!= second_idx {
                let first_expr = await_exprs[first_idx].clone();
                let second_expr = await_exprs[second_idx].clone();

                for item in &mut file.items {
                    if let syn::Item::Fn(func) = item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Await(await_expr) = expr {
                                    if stmt == &await_exprs[first_idx] {
                                        *stmt = second_expr.clone();
                                    } else if stmt == &await_exprs[second_idx] {
                                        *stmt = first_expr.clone();
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
        "The mutation operator targets async expressions within the same function scope. It swaps the positions of two await expressions, effectively interchanging the order in which the asynchronous operations are executed. This transformation can reveal issues related to the handling of asynchronous operations, such as bugs in the drop tracking mechanism or incorrect assumptions about the order of execution."
    }
}