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

pub struct Introduce_Nested_Question_Mark_Chains_220;

impl Mutator for Introduce_Nested_Question_Mark_Chains_220 {
    fn name(&self) -> &str {
        "Introduce_Nested_Question_Mark_Chains_220"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = NestedQuestionMarkVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces nested question mark operator chains on Result types by wrapping expressions in block expressions containing doubly-nested Results. It targets functions returning Result types, transforming expressions of type Result<T, E> into { Ok(Ok(original_expr)) }??. This creates complex control flow patterns that test the compiler's handling of nested error propagation, type inference in nested contexts, and the interaction between block expressions and the question mark operator."
    }
}

struct NestedQuestionMarkVisitor;

impl VisitMut for NestedQuestionMarkVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        if let ReturnType::Type(_, return_type) = &i.sig.output {
            if let Type::Path(TypePath { path, .. }) = &**return_type {
                if let Some(segment) = path.segments.last() {
                    if segment.ident == "Result" {
                        self.mutate_function_body(&mut i.block);
                    }
                }
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Call(call_expr) => {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    if let Some(segment) = path_expr.path.segments.last() {
                        if segment.ident == "Ok" || segment.ident == "Err" {
                            self.wrap_with_nested_block(expr);
                        }
                    }
                }
            }
            Expr::Match(match_expr) => {
                if let Some(arm) = match_expr.arms.first_mut() {
                    self.wrap_with_nested_block(&mut arm.body);
                }
            }
            Expr::If(if_expr) => {
                if let Some(last_stmt) = if_expr.then_branch.stmts.last_mut() {
                    if let Stmt::Expr(expr_stmt, _) = last_stmt {
                        self.wrap_with_nested_block(expr_stmt);
                    }
                }
            }
            Expr::Block(block_expr) => {
                if let Some(last_stmt) = block_expr.block.stmts.last_mut() {
                    if let Stmt::Expr(expr_stmt, _) = last_stmt {
                        self.wrap_with_nested_block(expr_stmt);
                    }
                }
            }
            _ => {}
2.        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl NestedQuestionMarkVisitor {
    fn mutate_function_body(&mut self, block: &mut Block) {
        let mut rng = thread_rng();
        let mut candidates = Vec::new();
        
        for stmt in &block.stmts {
            if let Stmt::Expr(expr, _) = stmt {
                self.collect_result_expressions(expr, &mut candidates);
            }
        }
        
        if !candidates.is_empty() {
            if let Some(target) = candidates.choose(&mut rng) {
                self.apply_mutation(target);
            }
        }
    }
    
    fn collect_result_expressions(&self, expr: &Expr, candidates: &mut Vec<Box<Expr>>) {
        match expr {
            Expr::Call(call_expr) => {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    if let Some(segment) = path_expr.path.segments.last() {
                        if segment.ident == "Ok" || segment.ident == "Err" {
                            candidates.push(Box::new(expr.clone()));
                        }
                    }
                }
                for arg in &call_expr.args {
                    self.collect_result_expressions(arg, candidates);
                }
            }
            Expr::Match(match_expr) => {
                self.collect_result_expressions(&match_expr.expr, candidates);
                for arm in &match_expr.arms {
                    self.collect_result_expressions(&arm.body, candidates);
                }
            }
            Expr::If(if_expr) => {
                self.collect_result_expressions(&if_expr.cond, candidates);
                for stmt in &if_expr.then_branch.stmts {
                    if let Stmt::Expr(expr_stmt, _) = stmt {
                        self.collect_result_expressions(expr_stmt, candidates);
                    }
                }
                if let Some((_, else_expr)) = &if_expr.else_branch {
                    self.collect_result_expressions(else_expr, candidates);
                }
            }
            Expr::Block(block_expr) => {
                for stmt in &block_expr.block.stmts {
                    if let Stmt::Expr(expr_stmt, _) = stmt {
                        self.collect_result_expressions(expr_stmt, candidates);
                    }
                }
            }
            Expr::Try(try_expr) => {
                self.collect_result_expressions(&try_expr.expr, candidates);
            }
            _ => {}
        }
    }
    
    fn apply_mutation(&self, target: &Box<Expr>) {
        let original_expr = &**target;
        let nested_expr: Expr = parse_quote! {
            { #original_expr }?
        };
        let double_nested_expr: Expr = parse_quote! {
            { Ok(#nested_expr) }?
        };
        
        if let Some(parent) = self.find_parent_mut(target) {
            match parent {
                Parent::Expr(expr) => *expr = double_nested_expr,
                Parent::Stmt(stmt) => {
                    if let Stmt::Expr(stmt_expr, _) = stmt {
                        *stmt_expr = double_nested_expr;
                    }
                }
                Parent::Arm(arm) => arm.body = Box::new(double_nested_expr),
            }
        }
    }
    
    fn wrap_with_nested_block(&self, expr: &mut Expr) {
        let original_expr = expr.clone();
        let nested_expr: Expr = parse_quote! {
            { #original_expr }?
        };
        let double_nested_expr: Expr = parse_quote! {
            { Ok(#nested_expr) }?
        };
        *expr = double_nested_expr;
    }
    
    fn find_parent_mut(&self, _target: &Box<Expr>) -> Option<Parent> {
        None
    }
}

enum Parent<'a> {
    Expr(&'a mut Expr),
    Stmt(&'a mut Stmt),
    Arm(&'a mut Arm),
}