use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAwait, ExprBlock, ExprClosure, ExprMethodCall, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Toggle_Async_Closure_Await_169;

impl Mutator for Toggle_Async_Closure_Await_169 {
    fn name(&self) -> &str {
        "Toggle_Async_Closure_Await_169"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ClosureVisitor::new();
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator toggles the async status of closures in Result combinators (and_then/map_err) and adjusts await usage to create type-checking discrepancies. This forces the compiler to reconcile futures and synchronous result chains during type inference, potentially exposing ICEs in async closure resolution and type snapshot systems."
    }
}

struct ClosureVisitor;

impl ClosureVisitor {
    fn new() -> Self {
        Self
    }

    fn process_closure(&self, closure: &mut ExprClosure) {
        // Toggle async status
        closure.asyncness = match closure.asyncness {
            Some(_) => None,
            None => Some(token::Async::default()),
        };

        // Adjust await usage in closure body
        if let Expr::Block(block) = &mut *closure.body {
            if let Some(last_expr) = &mut block.block.expr {
                let new_async_status = closure.asyncness.is_some();
                if new_async_status {
                    // Remove await from last expression
                    if let Expr::Await(await_expr) = &mut **last_expr {
                        *last_expr = *await_expr.base;
                    }
                } else {
                    // Add await to last expression
                    let base_expr = std::mem::replace(last_expr, Box::new(Expr::Verbatim(Default::default())));
                    let await_expr = ExprAwait {
                        attrs: vec![],
                        await_token: token::Await::default(),
                        base: base_expr,
                    };
                    *last_expr = Box::new(Expr::Await(Box::new(await_expr)));
                }
            }
        }
    }
}

impl<'ast> VisitMut for ClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::MethodCall(method_call) = expr {
            if method_call.method == "and_then" || method_call.method == "map_err" {
                if let Some(first_arg) = method_call.args.first_mut() {
                    if let Expr::Closure(closure) = **first_arg {
                        self.process_closure(closure);
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}