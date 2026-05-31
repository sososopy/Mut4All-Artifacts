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

pub struct Inject_Infinite_Recursion_326;

impl Mutator for Inject_Infinite_Recursion_326 {
    fn name(&self) -> &str {
        "Inject_Infinite_Recursion_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let func_name = func.sig.ident.clone();
                    let recursive_call: Expr = parse_quote! { #func_name() };
                    func.block.stmts.push(Stmt::Expr(recursive_call, None));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let func_name = func.sig.ident.clone();
                            let recursive_call: Expr = parse_quote! { #func_name() };
                            func.block.stmts.push(Stmt::Expr(recursive_call, None));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator injects an infinite recursive call into non-main functions and methods. This transformation aims to test the compiler's handling of recursion, stack overflow, and potential hangs during code execution. By forcing functions to call themselves indefinitely, it challenges the compiler's ability to optimize or detect recursion-related issues."
    }
}