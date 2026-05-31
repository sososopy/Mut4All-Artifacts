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

pub struct Replace_Const_Function_Call_With_Const_Value_372;

impl Mutator for Replace_Const_Function_Call_With_Const_Value_372 {
    fn name(&self) -> &str {
        "Replace_Const_Function_Call_With_Const_Value_372"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let Expr::Call(ExprCall { func, .. }) = &*item_const.expr {
                    if let Expr::Path(ExprPath { path, .. }) = &**func {
                        if path.segments.last().unwrap().ident.to_string() == "foo" {
                            // Replace the const function call with its return value
                            item_const.expr = parse_quote! { 5 };
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = &**func {
                                if path.segments.last().unwrap().ident.to_string() == "foo" {
                                    // Replace the const function call with its return value
                                    *expr = parse_quote! { 5 };
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const function calls in generic const expressions and replaces them with their return values. This transformation helps test how the compiler handles direct const values versus const function calls in generic const expressions, potentially exposing bugs like the one reported."
    }
}