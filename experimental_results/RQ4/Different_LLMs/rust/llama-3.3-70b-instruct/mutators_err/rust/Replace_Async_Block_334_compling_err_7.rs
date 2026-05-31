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

pub struct Replace_Async_Block_334;

impl Mutator for Replace_Async_Block_334 {
    fn name(&self) -> &str {
        "Replace_Async_Block_334"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut expressions = Vec::new();
        for item in &file.items {
            if let Item::Fn(func) = item {
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        expressions.push(expr.clone());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Async(ExprAsync { block, .. }) = expr {
                            if let Some(replacement) = expressions.choose(&mut thread_rng()) {
                                let new_block = Block {
                                    brace_token: block.brace_token,
                                    stmts: {
                                        let mut stmts: Punctuated<Stmt, Comma> = Punctuated::new();
                                        stmts.push(Stmt::Expr(replacement.clone(), None));
                                        stmts
                                    },
                                };
                                *expr = Expr::Async(ExprAsync {
                                    attrs: vec![],
                                    async_token: token::Async { span: Span::call_site() },
                                    capture: None,
                                    block: new_block,
                                });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the content of async blocks with a randomly chosen expression from the same function or module. This transformation aims to test the compiler's handling of async blocks and their interaction with other language features, potentially triggering bugs related to async block handling."
    }
}