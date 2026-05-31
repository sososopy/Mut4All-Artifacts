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

pub struct Replace_Box_With_Parentheses_236;

impl Mutator for Replace_Box_With_Parentheses_236 {
    fn name(&self) -> &str {
        "Replace_Box_With_Parentheses_236"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Call(ExprCall { expr, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = *expr {
                                if path.is_ident("box") {
                                    *expr = parse_quote! { (#expr.args.first().unwrap()) };
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Call(ExprCall { expr, .. }) = expr {
                                    if let Expr::Path(ExprPath { path, .. }) = *expr {
                                        if path.is_ident("box") {
                                            *expr = parse_quote! { (#expr.args.first().unwrap()) };
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
        "The mutation operator replaces instances of the box syntax with parentheses. This transformation aims to test the compiler's handling of removed box syntax and potentially trigger ICE bugs when the parser attempts to diagnose the removed syntax."
    }
}