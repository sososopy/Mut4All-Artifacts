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

pub struct Replace_Box_Leak_With_Async_Block_20;

impl Mutator for Replace_Box_Leak_With_Async_Block_20 {
    fn name(&self) -> &str {
        "Replace_Box_Leak_With_Async_Block_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in func.block.stmts.drain(..) {
                    match stmt {
                        Stmt::Local(ref local) => {
                            if is_valid_local(local) {
                                let async_expr = create_async_expr(local);
                                new_stmts.push(Stmt::Expr(async_expr, None));
                            } else {
                                new_stmts.push(stmt);
                            }
                        }
                        _ => {
                            new_stmts.push(stmt);
                        }
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies direct dereferences of Box::leak and wraps them in async move closures. This forces the compiler to capture the leaked reference, testing coroutine type inference and MIR handling under move semantics and async context."
    }
}

fn is_valid_local(local: &Local) -> bool {
    if let Some(init) = &local.init {
        if let Expr::Unary(expr_unary) = &*init.expr {
            if matches!(expr_unary.op, UnOp::Deref(_)) {
                is_box_leak_call(&expr_unary.expr)
            } else {
                false
            }
        } else {
            false
        }
    } else {
        false
    }
}

fn is_box_leak_call(expr: &Expr) -> bool {
    if let Expr::Call(expr_call) = expr {
        if let Expr::Path(expr_path) = &*expr_call.func {
            let segments = &expr_path.path.segments;
            if segments.len() == 2 {
                if segments[0].ident == "Box" && segments[1].ident == "leak" {
                    return true;
                }
            }
        }
    }
    false
}

fn create_async_expr(local: &Local) -> Expr {
    let pat = &local.pat;
    let local_init = local.init.as_ref().unwrap();
    if let Expr::Unary(expr_unary) = &**local_init.expr {
        let inner_expr = &*expr_unary.expr;
        let let_stmt = quote! {
            let #pat = #inner_expr;
        };
        parse_quote! {
            async move { #let_stmt }
        }
    } else {
        unreachable!()
    }
}