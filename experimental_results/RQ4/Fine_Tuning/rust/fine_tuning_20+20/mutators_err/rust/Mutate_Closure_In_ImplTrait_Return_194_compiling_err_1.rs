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

pub struct Mutate_Closure_In_ImplTrait_Return_194;

impl Mutator for Mutate_Closure_In_ImplTrait_Return_194 {
    fn name(&self) -> &str {
        "Mutate_Closure_In_ImplTrait_Return_194"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut closure_found = false;
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Some((_, expr)) = &local.init {
                                    if let Expr::Closure(closure) = expr.as_ref() {
                                        closure_found = true;
                                    }
                                }
                            }
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Closure(closure) = expr {
                                    closure_found = true;
                                }
                            }
                            if let Stmt::Semi(expr, _) = stmt {
                                if let Expr::Closure(closure) = expr {
                                    closure_found = true;
                                }
                            }
                        }
                        if !closure_found {
                            return;
                        }
                        let mut new_stmts = Vec::new();
                        for stmt in &item_fn.block.stmts {
                            match stmt {
                                Stmt::Local(local) => {
                                    if let Some((_, expr)) = &local.init {
                                        if let Expr::Closure(closure) = expr.as_ref() {
                                            let new_expr: Expr = parse_quote! {
                                                |_: &mut ()| {
                                                    let _ = y;
                                                    ()
                                                }
                                            };
                                            let new_local = Local {
                                                attrs: local.attrs.clone(),
                                                let_token: local.let_token,
                                                pat: local.pat.clone(),
                                                init: Some((local.init.as_ref().unwrap().0, Box::new(new_expr))),
                                                semi_token: local.semi_token,
                                            };
                                            new_stmts.push(Stmt::Local(new_local));
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    } else {
                                        new_stmts.push(stmt.clone());
                                    }
                                }
                                Stmt::Expr(expr, semi) => {
                                    if let Expr::Closure(closure) = expr {
                                        let new_expr: Expr = parse_quote! {
                                            |_: &mut ()| {
                                                let _ = y;
                                                ()
                                            }
                                        };
                                        new_stmts.push(Stmt::Expr(new_expr, *semi));
                                    } else {
                                        new_stmts.push(stmt.clone());
                                    }
                                }
                                Stmt::Semi(expr, semi) => {
                                    if let Expr::Closure(closure) = expr {
                                        let new_expr: Expr = parse_quote! {
                                            |_: &mut ()| {
                                                let _ = y;
                                                ()
                                            }
                                        };
                                        new_stmts.push(Stmt::Semi(new_expr, *semi));
                                    } else {
                                        new_stmts.push(stmt.clone());
                                    }
                                }
                                _ => {
                                    new_stmts.push(stmt.clone());
                                }
                            }
                        }
                        item_fn.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning `impl Trait` by modifying closures within their body to introduce an out-of-scope variable reference, aiming to stress-test the compiler's handling of closure scopes and lifetime inference in opaque return contexts."
    }
}