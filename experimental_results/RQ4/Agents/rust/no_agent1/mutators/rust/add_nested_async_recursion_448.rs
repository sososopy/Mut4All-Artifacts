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

pub struct Add_Nested_Async_Recursion_448;

impl Mutator for Add_Nested_Async_Recursion_448 {
    fn name(&self) -> &str {
        "Add_Nested_Async_Recursion_448"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" && func.sig.asyncness.is_some() {
                    let func_name = func.sig.ident.clone();
                    let async_block: Expr = parse_quote! {
                        async {
                            #func_name().await;
                        }
                    };
                    let recursive_call: Stmt = parse_quote! {
                        #async_block.await;
                    };
                    func.block.stmts.push(recursive_call);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            let func_name = func.sig.ident.clone();
                            let async_block: Expr = parse_quote! {
                                async {
                                    #func_name().await;
                                }
                            };
                            let recursive_call: Stmt = parse_quote! {
                                #async_block.await;
                            };
                            func.block.stmts.push(recursive_call);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a nested asynchronous recursive call within async functions. By embedding an async block that calls the function itself, it creates a deeply nested async recursion. This transformation aims to test the compiler's handling of async recursion and stack management, potentially triggering stack overflow or async task management issues."
    }
}