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

pub struct Modify_Closure_Return_Type_518;

impl Mutator for Modify_Closure_Return_Type_518 {
    fn name(&self) -> &str {
        "Modify_Closure_Return_Type_518"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Closure(closure) = &*init.expr {
                                    if closure.output == ReturnType::Default {
                                        let new_return_type: Type = parse_quote!(u32);
                                        let new_expr: Expr = parse_quote!(42);
                                        let mut new_block = closure.body.clone();
                                        if let Expr::Block(expr_block) = &mut *new_block {
                                            expr_block.block.stmts.push(Stmt::Expr(new_expr, None));
                                        }
                                        let new_closure = Expr::Closure(syn::ExprClosure {
                                            output: ReturnType::Type(token::RArrow::default(), Box::new(new_return_type)),
                                            ..closure.clone()
                                        });
                                        *stmt = Stmt::Local(syn::Local {
                                            init: Some(LocalInit {
                                                eq_token: token::Eq::default(),
                                                expr: Box::new(new_closure),
                                                diverge: None,
                                            }),
                                            ..local.clone()
                                        });
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