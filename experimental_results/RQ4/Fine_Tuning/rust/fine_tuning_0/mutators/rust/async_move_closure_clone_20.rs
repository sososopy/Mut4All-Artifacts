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

pub struct Async_Move_Closure_Clone_20;

impl Mutator for Async_Move_Closure_Clone_20 {
    fn name(&self) -> &str {
        "Async_Move_Closure_Clone_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = AsyncClosureVisitor::new();
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async move closures within chained futures operations and introduces an unnecessary clone of variables used in await operations. This transformation aims to test the compiler's handling of unnecessary data copies, especially in async contexts, potentially exposing borrow checker or state management issues."
    }
}

struct AsyncClosureVisitor;

impl AsyncClosureVisitor {
    fn new() -> Self {
        AsyncClosureVisitor
    }
}

impl VisitMut for AsyncClosureVisitor {
    fn visit_expr_closure_mut(&mut self, expr_closure: &mut ExprClosure) {
        if expr_closure.asyncness.is_some() && expr_closure.movability.is_some() {
            if let Expr::Block(expr_block) = &mut *expr_closure.body {
                let mut new_stmts = Vec::new();
                for stmt in &expr_block.block.stmts {
                    if let Stmt::Expr(Expr::Await(expr_await), _) = stmt {
                        if let Expr::MethodCall(expr_method_call) = &*expr_await.base {
                            if let Expr::Path(expr_path) = &*expr_method_call.receiver {
                                if let Some(ident) = expr_path.path.get_ident() {
                                    let clone_stmt: Stmt = parse_quote! {
                                        let #ident = #ident.clone();
                                    };
                                    new_stmts.push(clone_stmt);
                                    let new_await: Stmt = parse_quote! {
                                        #ident.method().await;
                                    };
                                    new_stmts.push(new_await);
                                    continue;
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                expr_block.block.stmts = new_stmts;
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, expr_closure);
    }
}