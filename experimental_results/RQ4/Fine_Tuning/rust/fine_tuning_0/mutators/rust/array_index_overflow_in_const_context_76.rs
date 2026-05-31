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

pub struct Array_Index_Overflow_In_Const_Context_76;

impl Mutator for Array_Index_Overflow_In_Const_Context_76 {
    fn name(&self) -> &str {
        "Array_Index_Overflow_In_Const_Context_76"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    let mut new_stmts = Vec::new();
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Unsafe(expr_unsafe), _) = stmt {
                            let inner_stmts = expr_unsafe.block.stmts.clone();
                            for inner_stmt in inner_stmts.iter() {
                                if let Stmt::Local(local) = inner_stmt {
                                    if let Some(LocalInit { expr, .. }) = &local.init {
                                        if let Expr::Array(expr_array) = &**expr {
                                            for access_stmt in inner_stmts.iter() {
                                                if let Stmt::Local(access_local) = access_stmt {
                                                    if let Some(LocalInit { expr: access_expr, .. }) = &access_local.init {
                                                        if let Expr::Index(expr_index) = &**access_expr {
                                                            if let Expr::Path(path) = &*expr_index.expr {
                                                                if path.path.segments.last().unwrap().ident == local.pat.to_token_stream().to_string() {
                                                                    let mut new_expr_index = expr_index.clone();
                                                                    new_expr_index.index = Box::new(Expr::Lit(ExprLit {
                                                                        attrs: vec![],
                                                                        lit: Lit::Int(LitInt::new("usize::MAX - 1", Span::call_site())),
                                                                    }));
                                                                    new_stmts.push(Stmt::Local(Local {
                                                                        init: Some(LocalInit {
                                                                            expr: Box::new(Expr::Index(new_expr_index)),
                                                                            ..access_local.init.clone().unwrap()
                                                                        }),
                                                                        ..access_local.clone()
                                                                    }));
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
                        new_stmts.push(stmt.clone());
                    }
                    item_fn.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets arrays within unsafe blocks in constant functions or contexts. It modifies the index used to access arrays to `usize::MAX - 1`, aiming to test the Rust compiler's handling of potential overflow conditions during constant evaluation."
    }
}