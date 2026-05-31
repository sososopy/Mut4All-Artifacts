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

pub struct Misplaced_Await_311;

impl Mutator for Misplaced_Await_311 {
    fn name(&self) -> &str {
        "Misplaced_Await_311"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct AsyncAwaitVisitor {
            found_async_fn: bool,
            async_calls: Vec<ExprCall>,
        }

        impl VisitMut for AsyncAwaitVisitor {
            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if i.sig.asyncness.is_some() {
                    self.found_async_fn = true;
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }

            fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
                if self.found_async_fn {
                    if let Expr::Path(ExprPath { path, .. }) = &*i.func {
                        if path.segments.last().unwrap().ident.to_string().contains("serve_udp_tunnel") {
                            self.async_calls.push(i.clone());
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, i);
            }
        }

        let mut visitor = AsyncAwaitVisitor {
            found_async_fn: false,
            async_calls: Vec::new(),
        };

        visitor.visit_file_mut(file);

        if let Some(async_call) = visitor.async_calls.first_mut() {
            let misplaced_await = parse_quote! {
                .await
            };

            let non_async_expr = parse_quote! {
                initial_exchange
            };

            let mut new_stmts = vec![];
            for item in &file.items {
                if let Item::Fn(func) = item {
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Call(call), _) = stmt {
                            if call == async_call {
                                let new_stmt: Stmt = parse_quote! {
                                    #non_async_expr #misplaced_await;
                                };
                                new_stmts.push(new_stmt);
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                }
            }
            if let Some(Item::Fn(func)) = file.items.first_mut() {
                func.block.stmts = new_stmts;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies asynchronous function calls within a function and misplaces the `.await` keyword. It removes the `.await` from the async call and places it after a non-async expression, introducing potential async handling errors."
    }
}