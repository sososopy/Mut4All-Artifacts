use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprAwait, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Async_Function_Await_Manipulation_16;

impl Mutator for Async_Function_Await_Manipulation_16 {
    fn name(&self) -> &str {
        "Async_Function_Await_Manipulation_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let mut await_exprs = vec![];
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Await(expr_await)) = stmt {
                            await_exprs.push(expr_await);
                        }
                    }
                    if await_exprs.len() > 1 {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(Expr::Assign(assign_expr)) = stmt {
                                if let Expr::Await(expr_await) = &*assign_expr.left {
                                    if let Expr::Reference(ref_expr) = &*expr_await.base {
                                        if let Expr::Path(expr_path) = &*ref_expr.expr {
                                            if expr_path.path.segments.last().unwrap().ident == "block" {
                                                let new_await_expr: Expr = parse_quote! {
                                                    async { &mut this.other_field }.await
                                                };
                                                assign_expr.left = Box::new(new_await_expr);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `async fn` functions with multiple `await` expressions. It specifically modifies an `await` expression on the left-hand side of an assignment that awaits a mutable reference, changing it to await a different, compatible field. This transformation explores alternative execution paths and tests the compiler's handling of assignment targets involving `await` expressions."
    }
}