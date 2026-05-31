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

pub struct Modify_Match_Guards_Async_604;

impl Mutator for Modify_Match_Guards_Async_604 {
    fn name(&self) -> &str {
        "Modify_Match_Guards_Async_604"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_another_async_function = false;
        
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "another_async_function" {
                    has_another_async_function = true;
                    break;
                }
            }
        }

        if !has_another_async_function {
            let new_fn: ItemFn = parse_quote! {
                async fn another_async_function() -> Option<i32> {
                    Some(2)
                }
            };
            file.items.push(Item::Fn(new_fn));
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                        for arm in &mut expr_match.arms {
                            if let Some((_, ref mut guard)) = arm.guard {
                                if let Expr::Let(expr_let) = &**guard {
                                    if let Expr::Await(expr_await) = &*expr_let.expr {
                                        if let Expr::Call(expr_call) = &*expr_await.base {
                                            if let Expr::Path(expr_path) = &*expr_call.func {
                                                if expr_path.path.segments.last().unwrap().ident == "async_function" {
                                                    let new_guard: Expr = parse_quote! {
                                                        if let Some(other_value) = another_async_function().await
                                                    };
                                                    *guard = Box::new(new_guard);
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}