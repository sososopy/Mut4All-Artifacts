use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAsync, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Mutator_Insert_AsyncClosure_Inside_AsyncBlock_23;

impl Mutator for Mutator_Insert_AsyncClosure_Inside_AsyncBlock_23 {
    fn name(&self) -> &str {
        "Mutator_Insert_AsyncClosure_Inside_AsyncBlock_23"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let block = &mut func.block;
                let mut visitor = AsyncBlockVisitor::new();
                visitor.visit_block_mut(block);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an async closure inside an async block, capturing local variables with `move` and awaiting its execution. This transformation introduces nested async constructs and closure captures, challenging the compiler's handling of HIR IDs, coverage instrumentation, and async context propagation. It leverages existing variables and functions to maximize code coverage and stress async-related compiler components."
    }
}

struct AsyncBlockVisitor;

impl AsyncBlockVisitor {
    fn new() -> Self {
        AsyncBlockVisitor
    }
}

impl VisitMut for AsyncBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_expr_async_mut(&mut self, expr: &mut ExprAsync) {
        if let Some(block) = &mut expr.block {
            let mut locals = Vec::new();
            for stmt in &mut block.stmts {
                if let Stmt::Local(local) = stmt {
                    locals.push(local);
                }
            }

            if !locals.is_empty() {
                let local = &locals[0];
                if let Some(init) = &local.init {
                    if let Some((_, expr)) = &init.expr {
                        if let Expr::Path(expr_path) = expr.as_ref() {
                            if let Some(ty) = local.ty.as_ref() {
                                let closure_name = Ident::new("closure", Span::call_site());
                                let param_name = Ident::new("y", Span::call_site());
                                let local_pat = &local.pat;

                                let process_call = parse_quote! {
                                    process(#local_pat, #param_name)
                                };

                                let closure_expr = parse_quote! {
                                    async move |#param_name: #ty| {
                                        #process_call
                                    }
                                };

                                let closure_assign = parse_quote! {
                                    let #closure_name = #closure_expr;
                                };

                                let await_call = parse_quote! {
                                    another_function(#closure_name).await
                                };

                                block.stmts.push(Stmt::Local(closure_assign));
                                block.stmts.push(Stmt::Expr(await_call, None));
                            }
                        }
                    }
                }
            }

            self.visit_block_mut(block);
        }
    }
}