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

pub struct Invalidate_Const_Loop_Return_Type_48;

impl Mutator for Invalidate_Const_Loop_Return_Type_48 {
    fn name(&self) -> &str {
        "Invalidate_Const_Loop_Return_Type_48"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    let mut visitor = LoopVisitor;
                    visitor.visit_block_mut(&mut func.block);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.constness.is_some() {
                            let mut visitor = LoopVisitor;
                            visitor.visit_block_mut(&mut method.block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies const loop expressions by replacing their final break value or loop condition with a subexpression that evaluates to an incompatible type. It collects variables declared within the loop's body and uses them in a comparison (e.g., `x > 0`) to produce a `bool`, which is incompatible with the expected return type of the loop. If no variables are found, it uses a literal (e.g., `true` or `42`) to ensure syntactic validity while creating a type mismatch. This transformation maximizes coverage by altering const evaluation paths and testing the compiler's type-checking for const contexts."
    }
}

struct LoopVisitor;

impl VisitMut for LoopVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::While(while_expr) = expr {
            let mut collector = VariableCollector::new();
            collector.visit_block(&while_expr.body);
            let vars = collector.get_vars();

            let body = &mut while_expr.body;
            if let Some(last_stmt) = body.stmts.last_mut() {
                if let Stmt::Expr(expr, _) = last_stmt {
                    if !vars.is_empty() {
                        let var = vars.iter().next().unwrap().clone();
                        let new_expr: Expr = parse_quote! { #var > 0 };
                        *expr = new_expr;
                    } else {
                        let new_expr: Expr = parse_quote! { true };
                        *expr = new_expr;
                    }
                }
            }
        } else if let Expr::ForLoop(for_expr) = expr {
            let mut collector = VariableCollector::new();
            collector.visit_block(&for_expr.body);
            let vars = collector.get_vars();

            let body = &mut for_expr.body;
            if let Some(last_stmt) = body.stmts.last_mut() {
                if let Stmt::Expr(expr, _) = last_stmt {
                    if !vars.is_empty() {
                        let var = vars.iter().next().unwrap().clone();
                        let new_expr: Expr = parse_quote! { #var > 0 };
                        *expr = new_expr;
                    } else {
                        let new_expr: Expr = parse_quote! { 42 };
                        *expr = new_expr;
                    }
                }
            }
        } else if let Expr::Loop(loop_expr) = expr {
            let mut collector = VariableCollector::new();
            collector.visit_block(&loop_expr.body);
            let vars = collector.get_vars();

            let body = &mut loop_expr.body;
            if let Some(last_stmt) = body.stmts.last_mut() {
                if let Stmt::Expr(expr, _) = last_stmt {
                    if !vars.is_empty() {
                        let var = vars.iter().next().unwrap().clone();
                        let new_expr: Expr = parse_quote! { #var > 0 };
                        *expr = new_expr;
                    } else {
                        let new_expr: Expr = parse_quote! { true };
                        *expr = new_expr;
                    }
                }
            }
        }

        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

struct VariableCollector {
    vars: HashSet<Ident>,
}

impl VariableCollector {
    fn new() -> Self {
        Self {
            vars: HashSet::new(),
        }
    }

    fn get_vars(&self) -> &HashSet<Ident> {
        &self.vars
    }
}

impl Visit<'_> for VariableCollector {
    fn visit_local(&mut self, local: &Local) {
        if let Pat::Ident(pat) = &local.pat {
            self.vars.insert(pat.ident.clone());
        }
        syn::visit::visit_local(self, local);
    }
}