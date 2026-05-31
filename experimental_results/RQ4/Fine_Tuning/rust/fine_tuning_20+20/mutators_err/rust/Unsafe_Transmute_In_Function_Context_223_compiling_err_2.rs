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

pub struct Unsafe_Transmute_In_Function_Context_223;

impl Mutator for Unsafe_Transmute_In_Function_Context_223 {
    fn name(&self) -> &str {
        "Unsafe_Transmute_In_Function_Context_223"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut option_type = None;
                let mut transmute_ident = None;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Call(expr_call) = init.expr.as_ref() {
                                if let Expr::Path(expr_path) = expr_call.func.as_ref() {
                                    if expr_path.path.is_ident("Some")
                                        || expr_path.path.is_ident("None")
                                    {
                                        if let Pat::Ident(pat_ident) = &local.pat {
                                            transmute_ident = Some(pat_ident.ident.clone());
                                        }
                                        if let Some(pat_type) = &local.ty {
                                            if let Type::Path(type_path) = &*pat_type.ty {
                                                option_type = Some(type_path.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(option_type) = option_type {
                    let option_type_str = option_type.to_token_stream().to_string();
                    let return_type = if option_type_str.contains("u8") {
                        parse_quote!(u8)
                    } else if option_type_str.contains("u16") {
                        parse_quote!(u16)
                    } else if option_type_str.contains("u32") {
                        parse_quote!(u32)
                    } else if option_type_str.contains("u64") {
                        parse_quote!(u64)
                    } else if option_type_str.contains("i8") {
                        parse_quote!(i8)
                    } else if option_type_str.contains("i16") {
                        parse_quote!(i16)
                    } else if option_type_str.contains("i32") {
                        parse_quote!(i32)
                    } else if option_type_str.contains("i64") {
                        parse_quote!(i64)
                    } else {
                        return;
                    };
                    let transmute_fn: ItemFn = parse_quote! {
                        unsafe fn transmute_option(option: #option_type) -> #return_type {
                            std::mem::transmute(option)
                        }
                    };
                    let transmute_call: Expr = parse_quote! {
                        transmute_option(#transmute_ident)
                    };
                    let mut new_stmts = Vec::new();
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Unsafe(expr_unsafe) = init.expr.as_ref() {
                                    if let Expr::Call(expr_call) = expr_unsafe.block.stmts[0]
                                        .clone()
                                        .into_expr()
                                    {
                                        if let Expr::Path(expr_path) = expr_call.func.as_ref() {
                                            if expr_path.path.segments.len() == 3
                                                && expr_path.path.segments[0].ident == "std"
                                                && expr_path.path.segments[1].ident == "mem"
                                                && expr_path.path.segments[2].ident == "transmute"
                                            {
                                                let new_stmt = Stmt::Local(syn::Local {
                                                    attrs: local.attrs.clone(),
                                                    let_token: local.let_token,
                                                    pat: local.pat.clone(),
                                                    init: Some(syn::LocalInit {
                                                        eq_token: init.eq_token.clone(),
                                                        expr: transmute_call.clone(),
                                                        diverge: init.diverge.clone(),
                                                    }),
                                                    semi_token: local.semi_token,
                                                    ty: local.ty.clone(),
                                                });
                                                new_stmts.push(new_stmt);
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
                    file.items.push(Item::Fn(transmute_fn));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that transmute an Option<T> to a basic numeric type (e.g., u64) within the function body. It refactors the code by extracting the transmutation logic into a new unsafe function, transmute_option, which takes the Option<T> and returns the desired numeric type. The original transmutation is replaced with a call to this helper function. This transformation aims to test the compiler's handling of unsafe code and function extraction, potentially exposing issues in code generation or optimization passes."
    }
}