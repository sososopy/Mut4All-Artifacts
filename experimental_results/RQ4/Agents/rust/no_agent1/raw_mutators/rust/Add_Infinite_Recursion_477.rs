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

pub struct Add_Infinite_Recursion_477;

impl Mutator for Add_Infinite_Recursion_477 {
    fn name(&self) -> &str {
        "Add_Infinite_Recursion_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let func_name = func.sig.ident.clone();
                let recurse_expr: Expr = parse_quote! {
                    #func_name()
                };
                func.block.stmts.push(Stmt::Expr(recurse_expr, None));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let func_name = func.sig.ident.clone();
                        let recurse_expr: Expr = parse_quote! {
                            #func_name()
                        };
                        func.block.stmts.push(Stmt::Expr(recurse_expr, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends a recursive call to each non-main function and method, creating an infinite recursion scenario. This transformation aims to test the compiler's ability to handle stack overflow conditions and recursion limits, potentially triggering ICEs or hangs due to unbounded recursion."
    }
}