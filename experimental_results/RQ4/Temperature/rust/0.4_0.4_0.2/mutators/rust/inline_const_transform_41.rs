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

pub struct Inline_Const_Transform_41;

impl Mutator for Inline_Const_Transform_41 {
    fn name(&self) -> &str {
        "Inline_Const_Transform_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trivial_computation_fn: ItemFn = parse_quote! {
            fn trivial_computation(x: i32) -> i32 {
                x + 1
            }
        };

        let mut inserted_fn = false;
        let mut new_items = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !inserted_fn {
                    new_items.push(Item::Fn(trivial_computation_fn.clone()));
                    inserted_fn = true;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            if let Expr::Block(expr_block) = &**expr {
                                if expr_block.block.stmts.len() == 1 {
                                    if let Stmt::Expr(inner_expr, _) = &expr_block.block.stmts[0] {
                                        if let Expr::Lit(_) = inner_expr {
                                            let new_expr: Expr = parse_quote! {
                                                trivial_computation(#inner_expr)
                                            };
                                            let new_stmt = Stmt::Expr(new_expr, None);
                                            let mut new_block = expr_block.clone();
                                            new_block.block.stmts[0] = new_stmt;
                                            *stmt = Stmt::Local(Local {
                                                init: Some(LocalInit {
                                                    expr: Box::new(Expr::Block(new_block)),
                                                    ..local.init.clone().unwrap()
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

        file.items.append(&mut new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets inline constant blocks within functions. It modifies these blocks to include a call to a trivial computation function, which performs a simple arithmetic operation. The mutation introduces a function call within the constant block to test the compiler's handling of inline constants and intrinsic operations."
    }
}