use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Boxed_Trait_Ref_In_Async_Closure_653;

impl Mutator for Boxed_Trait_Ref_In_Async_Closure_653 {
    fn name(&self) -> &str {
        "Boxed_Trait_Ref_In_Async_Closure_653"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Async(expr_async), _) = stmt {
                        let mut new_stmts = Vec::new();
                        for stmt in &mut expr_async.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Some(LocalInit { expr, .. }) = &local.init {
                                    if let Expr::Await(expr_await) = &**expr {
                                        if let Expr::MethodCall(method_call) = &*expr_await.base {
                                            if method_call.method == "start_loading" {
                                                let loader_ident = &method_call.receiver;
                                                let deref_stmt: Stmt = parse_quote! {
                                                    let dereferenced_loader: &dyn ResourceLoader = &*#loader_ident;
                                                };
                                                let rebox_stmt: Stmt = parse_quote! {
                                                    let reboxed_loader: Box<dyn ResourceLoader> = Box::new(*dereferenced_loader);
                                                };
                                                let new_expr: Expr = parse_quote! {
                                                    reboxed_loader.start_loading().await
                                                };
                                                local.init = Some(LocalInit {
                                                    eq_token: token::Eq::default(),
                                                    expr: Box::new(new_expr),
                                                    diverge: None,
                                                });
                                                new_stmts.push(deref_stmt);
                                                new_stmts.push(rebox_stmt);
                                            }
                                        }
                                    }
                                }
                            }
                            new_stmts.push(stmt.clone());
                        }
                        expr_async.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures using boxed trait object references. It replaces direct usage with a dereferenced and re-boxed version, testing the compiler's handling of trait object references in async contexts. This transformation challenges the compiler's ability to manage trait object lifetimes and conversions, potentially exposing issues in async trait handling."
    }
}