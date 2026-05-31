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

pub struct Add_Nested_Async_Blocks_462;

impl Mutator for Add_Nested_Async_Blocks_462 {
    fn name(&self) -> &str {
        "Add_Nested_Async_Blocks_462"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let original_block = func.block.clone();
                let nested_async_block: Expr = parse_quote! {
                    async {
                        let _ = async {
                            #original_block
                        }.await;
                    }
                };
                func.block.stmts.clear();
                func.block.stmts.push(Stmt::Expr(nested_async_block, None));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let original_block = func.block.clone();
                        let nested_async_block: Expr = parse_quote! {
                            async {
                                let _ = async {
                                    #original_block
                                }.await;
                            }
                        };
                        func.block.stmts.clear();
                        func.block.stmts.push(Stmt::Expr(nested_async_block, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator wraps the body of non-main functions and methods in nested async blocks. This transformation introduces complex async control flow, challenging the compiler's async handling, particularly around nested await points and future resolution. It aims to uncover bugs in async transformation and execution, potentially leading to ICEs or runtime panics."
    }
}