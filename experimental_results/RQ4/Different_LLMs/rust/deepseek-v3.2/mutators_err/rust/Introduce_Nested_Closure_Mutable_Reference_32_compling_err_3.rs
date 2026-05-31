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

pub struct Introduce_Nested_Closure_Mutable_Reference_32;

impl Mutator for Introduce_Nested_Closure_Mutable_Reference_32 {
    fn name(&self) -> &str {
        "Introduce_Nested_Closure_Mutable_Reference_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = ClosureMutationVisitor {
            rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies mutable variable bindings and closure definitions within the same scope. It modifies the closure to capture the mutable variable by mutable reference, then inserts a use of that variable (e.g., a method call or assignment) after the closure definition but before any invocation, all placed inside a match or if-let expression. This creates complex lifetime and mutable aliasing scenarios that stress the borrow checker's region inference across nested scopes and conditional branches, potentially leading to internal compiler errors."
    }
}

struct ClosureMutationVisitor {
    rng: rand::rngs::ThreadRng,
    mutated: bool,
}

impl ClosureMutationVisitor {
    fn find_mutable_variable(&self, block: &syn::Block) -> Option<syn::Ident> {
        for stmt in &block.stmts {
            if let syn::Stmt::Local(local) = stmt {
                if let syn::Pat::Ident(pat_ident) = &local.pat {
                    if pat_ident.mutability.is_some() {
                        return Some(pat_ident.ident.clone());
                    }
                }
            }
        }
        None
    }

    fn find_closure<'a>(&self, block: &'a syn::Block) -> Option<&'a syn::ExprClosure> {
        for stmt in &block.stmts {
            if let syn::Stmt::Expr(expr, _) = stmt {
                if let syn::Expr::Closure(closure) = expr {
                    return Some(closure);
                }
            }
        }
        None
    }

    fn wrap_in_match(&self, stmts: Vec<syn::Stmt>) -> syn::ExprMatch {
        let arms = Vec::new();
        let match_expr = syn::ExprMatch {
            attrs: Vec::new(),
            match_token: token::Match::default(),
            expr: Box::new(syn::Expr::Lit(syn::ExprLit {
                attrs: Vec::new(),
               2lit: syn::Lit::Bool(syn::LitBool {
                    value: true,
                    span: Span::call_site(),
                }),
            })),
            brace_token: token::Brace::default(),
            arms,
        };
        match_expr
    }
}

impl VisitMut for ClosureMutationVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Match(expr_match) = expr {
            for arm in &mut expr_match.arms {
                if let syn::Expr::Block(block) = &mut *arm.body {
                    let mutable_var = self.find_mutable_variable(&block.block);
                    let closure = self.find_closure(&block.block);
                    if let (Some(var_ident), Some(closure_expr)) = (mutable_var, closure) {
                        if !self.mutated && self.rng.gen_bool(0.5) {
                            let mut new_stmts = Vec::new();
                            for stmt in &block.block.stmts {
                                if let syn::Stmt::Expr(expr, _) = stmt {
                                    if let syn::Expr::Closure(_) = expr {
                                        let mut mutated_closure = closure_expr.clone();
                                        let closure_body = &mut mutated_closure.body;
                                        if let syn::Expr::Block(closure_block) = &mut **closure_body {
                                            closure_block.block.stmts.insert(0, parse_quote! { #var_ident += 1; });
                                        }
                                        new_stmts.push(syn::Stmt::Expr(syn::Expr::Closure(mutated_closure), None));
                                        new_stmts.push(parse_quote! { #var_ident = #var_ident.clone(); });
                                    } else {
                                        new_stmts.push(stmt.clone());
                                    }
                                } else {
                                    new_stmts.push(stmt.clone());
                                }
                            }
                            block.block.stmts = new_stmts;
                            self.mutated = true;
                        }
                    }
                }
            }
        }
        if let syn::Expr::If(expr_if) = expr {
            if let Some(block) = &mut expr_if.then_branch {
                let mutable_var = self.find_mutable_variable(&block.block);
                let closure = self.find_closure(&block.block);
                if let (Some(var_ident), Some(closure_expr)) = (mutable_var, closure) {
                    if !self.mutated && self.rng.gen_bool(0.5) {
                        let mut new_stmts = Vec::new();
                        for stmt in &block.block.stmts {
                            if let syn::Stmt::Expr(expr, _) = stmt {
                                if let syn::Expr::Closure(_) = expr {
                                    let mut mutated_closure = closure_ex2pr.clone();
                                    let closure_body = &mut mutated_closure.body;
                                    if let syn::Expr::Block(closure_block) = &mut **closure_body {
                                        closure_block.block.stmts.insert(0, parse_quote! { #var_ident += 1; });
                                    }
                                    new_stmts.push(syn::Stmt::Expr(syn::Expr::Closure(mutated_closure), None));
                                    new_stmts.push(parse_quote! { #var_ident = #var_ident.clone(); });
                                } else {
                                    new_stmts.push(stmt.clone());
                                }
                            } else {
                                new_stmts.push(stmt.clone());
                            }
                        }
                        block.block.stmts = new_stmts;
                        self.mutated = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}