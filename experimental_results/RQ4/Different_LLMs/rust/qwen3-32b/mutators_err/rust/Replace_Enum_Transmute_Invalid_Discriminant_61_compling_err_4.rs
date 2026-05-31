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

pub struct Replace_Enum_Transmute_Invalid_Discriminant_61;

impl Mutator for Replace_Enum_Transmute_Invalid_Discriminant_61 {
    fn name(&self) -> &str {
        "Replace_Enum_Transmute_Invalid_Discriminant_61"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let Expr::Unsafe(unsafe_expr) = item_const.expr.as_mut() {
                    if let Some(stmt) = unsafe_expr.block.stmts.first_mut() {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Call(call_expr) = expr.as_mut() {
                                if is_mem_transmute(&call_expr.func) {
                                    if let Some(arg) = call_expr.args.first_mut() {
                                        modify_argument(arg);
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

fn is_mem_transmute(func: &Box<Expr>) -> bool {
    if let Expr::Path(path_expr) = &**func {
        let path = &path_expr.path;
        if path.segments.len() == 2 {
            if path.segments[0].ident == "mem" && path.segments[1].ident == "transmute" {
                return true;
            }
        }
    }
    false
}

fn modify_argument(arg: &mut Expr) {
    match arg {
        Expr::Lit(lit_expr) => {
            if let Lit::Int(int_lit) = &lit_expr.lit {
                let original_value = int_lit.base10_parse::<u64>().unwrap();
                let new_value = original_value + 1;
                let suffix = int_lit.suffix();
                let new_value_str = format!("{}{}", new_value, suffix);
                let new_lit = LitInt::new(&new_value_str, int_lit.span());
                lit_expr.lit = new_lit.into();
            }
        }
        _ => {
            *arg = parse_quote! { 1 };
        }
    }
}