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

pub struct Modify_Array_Size_In_Async_Functions_17;

impl Mutator for Modify_Array_Size_In_Async_Functions_17 {
    fn name(&self) -> &str {
        "Modify_Array_Size_In_Async_Functions_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_none() {
                    continue;
                }
                let mut has_array = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Array(_) = &**expr {
                                has_array = true;
                                break;
                            }
                        }
                    }
                }
                if !has_array {
                    continue;
                }
                let mut has_await = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Await(_) = expr {
                            has_await = true;
                            break;
                        }
                    }
                }
                if !has_await {
                    continue;
                }
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((eq, expr)) = &local.init {
                            if let Expr::Array(expr_array) = &**expr {
                                if let Expr::Lit(expr_lit) = &expr_array.elems[0] {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        if lit_int.base10_parse::<usize>().is_ok() {
                                            let new_local = Local {
                                                attrs: local.attrs.clone(),
                                                let_token: local.let_token,
                                                pat: local.pat.clone(),
                                                init: Some((
                                                    eq.clone(),
                                                    Box::new(Expr::Array(ExprArray {
                                                        attrs: expr_array.attrs.clone(),
                                                        bracket_token: expr_array.bracket_token,
                                                        elems: {
                                                            let mut elems = Punctuated::new();
                                                            elems.push(Expr::Lit(ExprLit {
                                                                attrs: expr_lit.attrs.clone(),
                                                                lit: syn::Lit::Int(syn::LitInt::new(
                                                                    lit_int.base10_digits(),
                                                                    lit_int.span(),
                                                                )),
                                                            }));
                                                            elems.push(Expr::Path(ExprPath {
                                                                attrs: Vec::new(),
                                                                qself: None,
                                                                path: syn::Path {
                                                                    leading_colon: None,
                                                                    segments: {
                                                                        let mut segments =
                                                                            Punctuated::new();
                                                                        segments.push(
                                                                            PathSegment {
                                                                                ident: Ident::new(
                                                                                    "usize",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                                arguments:
                                                                                    PathArguments::None,
                                                                            },
                                                                        );
                                                                        segments.push(
                                                                            PathSegment {
                                                                                ident: Ident::new(
                                                                                    "MAX",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                                arguments:
                                                                                    PathArguments::None,
                                                                            },
                                                                        );
                                                                        segments
                                                                    },
                                                                },
                                                            }));
                                                            elems
                                                        },
                                                    })),
                                                )),
                                                semi_token: local.semi_token,
                                            };
                                            new_stmts.push(Stmt::Local(new_local));
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions containing array initializations and async constructs like .await expressions. It modifies the size of arrays initialized with usize elements to usize::MAX, leveraging the maximum possible size for stress testing the compiler's handling of large allocations and async-related code paths."
    }
}