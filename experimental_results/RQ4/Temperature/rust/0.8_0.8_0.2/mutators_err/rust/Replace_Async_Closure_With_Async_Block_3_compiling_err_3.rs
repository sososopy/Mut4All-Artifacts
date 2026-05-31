use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprAsync, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Async_Closure_With_Async_Block_3;

impl Mutator for Replace_Async_Closure_With_Async_Block_3 {
    fn name(&self) -> &str {
        "Replace_Async_Closure_With_Async_Block_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = AsyncClosureReplacer;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async closures, a feature prone to bugs due to its unstable nature. By replacing async closures with stable async blocks, we aim to stabilize the codebase while potentially revealing discrepancies in how the compiler handles these constructs."
    }
}

struct AsyncClosureReplacer;

impl VisitMut for AsyncClosureReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(expr_closure) = expr {
            if expr_closure.asyncness.is_some() {
                let async_expr = Expr::Async(ExprAsync {
                    attrs: expr_closure.attrs.clone(),
                    async_token: expr_closure.asyncness.take().unwrap(),
                    capture: expr_closure.capture.take(),
                    block: syn::Block {
                        brace_token: syn::token::Brace::default(),
                        stmts: vec![syn::Stmt::Expr(*expr_closure.body)],
                    },
                });
                *expr = async_expr;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}