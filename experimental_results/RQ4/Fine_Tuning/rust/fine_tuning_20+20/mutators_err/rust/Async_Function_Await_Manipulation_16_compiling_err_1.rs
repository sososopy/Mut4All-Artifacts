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

pub struct Async_Function_Await_Manipulation_16;

impl Mutator for Async_Function_Await_Manipulation_16 {
    fn name(&self) -> &str {
        "Async_Function_Await_Manipulation_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }

                let mut await_exprs = Vec::new();
                for (index, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Await(_) = expr {
                            await_exprs.push(index);
                        }
                    }
                }

                if await_exprs.len() < 2 {
                    continue;
                }

                let mut target_index = None;
                for &index in &await_exprs {
                    if let Stmt::Expr(Expr::Await(expr_await), _) =
                        &item_fn.block.stmts[index]
                    {
                        if let Expr::Async(expr_async) = &*expr_await.base {
                            if let Expr::Reference(expr_ref) = &*expr_async.block.stmts[0]
                                .as_expr()
                                .unwrap()
                            {
                                if let Expr::Reference(_) = &*expr_ref.expr {
                                    target_index = Some(index);
                                    break;
                                }
                            }
                        }
                    }
                }

                if let Some(index) = target_index {
                    if let Stmt::Expr(Expr::Await(expr_await), _) =
                        &mut item_fn.block.stmts[index]
                    {
                        if let Expr::Async(expr_async) = &*expr_await.base {
                            if let Expr::Reference(expr_ref) = &*expr_async.block.stmts[0]
                                .as_expr()
                                .unwrap()
                            {
                                if let Expr::Reference(inner_ref) = &*expr_ref.expr {
                                    if let Expr::Path(expr_path) = &*inner_ref.expr {
                                        let segments = &expr_path.path.segments;
                                        if segments.len() == 2 {
                                            let new_expr: Expr = parse_quote! {
                                                &mut #segments
                                            };
                                            expr_async.block.stmts[0] =
                                                Stmt::Expr(new_expr, None);
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
        "This mutator targets async functions with multiple await expressions. It identifies an await expression on the left-hand side of an assignment that awaits a mutable reference, then modifies it to await a different but compatible field or variable, ensuring the assignment remains valid. This transformation aims to explore alternative execution paths and test the compiler's handling of await expressions in assignment contexts."
    }
}