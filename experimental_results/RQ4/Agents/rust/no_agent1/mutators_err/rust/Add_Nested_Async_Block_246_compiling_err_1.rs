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

pub struct Add_Nested_Async_Block_246;

impl Mutator for Add_Nested_Async_Block_246 {
    fn name(&self) -> &str {
        "Add_Nested_Async_Block_246"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let async_block: Expr = parse_quote! {
                    async {
                        async {
                            // Nested async block
                        }.await;
                    }
                };
                let mut new_block = func.block.clone();
                new_block.stmts.clear();
                new_block.stmts.push(Stmt::Expr(async_block));
                func.block = new_block;
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let async_block: Expr = parse_quote! {
                            async {
                                async {
                                    // Nested async block
                                }.await;
                            }
                        };
                        let mut new_block = func.block.clone();
                        new_block.stmts.clear();
                        new_block.stmts.push(Stmt::Expr(async_block));
                        func.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a nested async block into the body of non-main functions and methods. This transformation introduces complex async control flow and awaits, potentially leading to compiler ICEs or deep bugs related to async/await state machine generation, borrow checking, and type inference in async contexts."
    }
}