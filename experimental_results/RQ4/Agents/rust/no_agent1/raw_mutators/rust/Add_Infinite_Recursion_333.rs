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

pub struct Add_Infinite_Recursion_333;

impl Mutator for Add_Infinite_Recursion_333 {
    fn name(&self) -> &str {
        "Add_Infinite_Recursion_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let func_name = func.sig.ident.clone();
                let call_expr: Expr = parse_quote! { #func_name() };
                let mut new_block = func.block.clone();
                new_block.stmts.push(Stmt::Expr(call_expr, None));
                func.block = new_block;
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let func_name = func.sig.ident.clone();
                        let call_expr: Expr = parse_quote! { #func_name() };
                        let mut new_block = func.block.clone();
                        new_block.stmts.push(Stmt::Expr(call_expr, None));
                        func.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an infinite recursion by appending a self-call at the end of each non-main function and method. This transformation aims to test the compiler's handling of recursion limits, stack overflow detection, and error reporting mechanisms under extreme conditions. It may trigger ICEs or other bugs related to deep recursion handling."
    }
}