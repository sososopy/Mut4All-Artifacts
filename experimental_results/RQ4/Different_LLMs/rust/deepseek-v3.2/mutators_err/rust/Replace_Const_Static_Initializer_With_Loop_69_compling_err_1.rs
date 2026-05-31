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

pub struct Replace_Const_Static_Initializer_With_Loop_69;

impl Mutator for Replace_Const_Static_Initializer_With_Loop_69 {
    fn name(&self) -> &str {
        "Replace_Const_Static_Initializer_With_Loop_69"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Const(item_const) => {
                    if !is_diverging_expr(&item_const.expr) {
                        item_const.expr = parse_quote!(loop {});
                    }
                }
                Item::Static(item_static) => {
                    if !is_diverging_expr(&item_static.expr) {
                        item_static.expr = parse_quote!(loop {});
                    }
                }
                Item::Fn(item_fn) => {
                    if item_fn.sig.constness.is_some() {
                        let mut visitor = ConstFnVisitor::new();
                        visitor.visit_item_fn_mut(item_fn);
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant items (const) and static items (static) within const contexts, specifically const fn bodies and other const contexts where loops are allowed. It replaces non-diverging initializer expressions with an infinite loop expression (loop {}). This transformation stresses the compiler's ability to manage const evaluation control flow and type checking in edge cases where the value never resolves, aiming to expose bugs similar to the reported ICE where internal data structures become inconsistent with divergent constant expressions."
    }
}

fn is_diverging_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Loop(expr_loop) => expr_loop.body.stmts.is_empty(),
        Expr::Break(_) => true,
        Expr::Return(_) => true,
        Expr::Macro(macro_expr) => {
            let path = &macro_expr.path;
            path.is_ident("panic")
        }
        _ => false,
    }
}

struct ConstFnVisitor;

impl ConstFnVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for ConstFnVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Const(expr_const) => {
                if !is_diverging_expr(&expr_const.expr) {
                    expr_const.expr = parse_quote!(loop {});
                }
            }
            Expr::Static(expr_static) => {
                if !is_diverging_expr(&expr_static.expr) {
                    expr_static.expr = parse_quote!(loop {});
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}