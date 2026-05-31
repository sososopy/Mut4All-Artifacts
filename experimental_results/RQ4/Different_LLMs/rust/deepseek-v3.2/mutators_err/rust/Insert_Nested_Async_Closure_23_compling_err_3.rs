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

pub struct Insert_Nested_Async_Closure_23;

impl Mutator for Insert_Nested_Async_Closure_23 {
    fn name(&self) -> &str {
        "Insert_Nested_Async_Closure_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut visitor = AsyncClosureVisitor::new();
                    visitor.visit_block_mut(&mut item_fn.block);
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            let mut visitor = AsyncClosureVisitor::new();
                            visitor.visit_block_mut(&mut func.block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a nested async closure within existing async items. It targets async functions and async blocks, identifying a statement or final expression and inserting a new async closure statement before it. The closure captures variables from the outer scope using move if needed, with a simple body like a unit expression. This increases nesting depth and creates additional closure nodes to stress the compiler's internal indexing for coverage instrumentation."
    }
}

struct AsyncClosureVisitor {
    captured_vars: Vec<Ident>,
}

impl AsyncClosureVisitor {
    fn new() -> Self {
        AsyncClosureVisitor {
            captured_vars: Vec::new(),
        }
    }

    fn collect_captured_vars(&mut self, block: &syn::Block) {
        let mut var_collector = VarCollector::new();
        var_collector.visit_block(block);
        self.captured_vars = var_collector.vars;
    }

    fn create_async_closure(&self) -> syn::Expr {
        let closure_body = if self.captured_vars.is_empty() {
            parse_quote!({})
        } else {
            let captures = self.captured_vars.iter().map(|ident| {
                parse_quote! { let _ = #ident; }
            }).collect::<Vec<syn::Stmt>>();
            let block = syn::Block {
                brace_token: token::Brace::default(),
                stmts: captures,
            };
            parse_quote! { #block }
        };

        let closure = syn::ExprClosure {
            attrs: Vec::new(),
            asyncness: Some(token::Async::default()),
            capture: Some(token::Move::default()),
            constness: None,
            lifetimes: None,
            movability: None,
            or1_token: token::Or::default(),
            or2_token: token::Or::default(),
            inputs: Punctuated::new(),
            output: ReturnType::Default,
            body: Box::new(closure_body),
        };

        parse_quote! { (#closure)() }
    }
}

impl VisitMut for AsyncClosureVisitor {
    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        self.collect_captured_vars(block);

        if block.stmts.len() > 0 {
            let insert_index = if let Some(last_stmt) = block.stmts.last() {
                if matches!(last_stmt, Stmt::Expr(_, None)) {
                    block.stmts.len() - 1
                } else {
                    block.stmts.len()
                }
            } else {
                block.stmts.len()
            };

            let async_closure_expr = self.create_async_closure();
            let async_closure_stmt = Stmt::Expr(
                parse_quote! { #async_closure_expr.await },
                None
            );

            block.stmts.insert(insert_index, async_closure_stmt);
        }
    }
}

struct VarCollector {
    vars: Vec<Ident>,
}

impl VarCollector {
    fn new() -> Self {
        VarCollector { vars: Vec::new() }
    }
}

impl Visit<'_> for VarCollector {
    fn visit_local(&mut self, local: &syn::Local) {
        if let Pat::Ident(pat_ident) = &local.pat {
            self.vars.push(pat_ident.ident.clone());
        }
        syn::visit::visit_local(self, local);
    }

    fn visit_pat(&mut self, pat: &syn::Pat) {
        if let Pat::Ident(pat_ident) = pat {
            self.vars.push(pat_ident.ident.clone());
        }
        syn::visit::visit_pat(self, pat);
    }
}