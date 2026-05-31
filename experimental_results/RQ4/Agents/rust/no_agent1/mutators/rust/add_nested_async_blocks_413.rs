use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ItemFn, Stmt, parse_quote,
    visit_mut::VisitMut, token::Semi,
};

use crate::mutator::Mutator;

pub struct Add_Nested_Async_Blocks_413;

impl Mutator for Add_Nested_Async_Blocks_413 {
    fn name(&self) -> &str {
        "Add_Nested_Async_Blocks_413"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let nested_async_block: Expr = parse_quote! {
                        async {
                            async {
                                async {
                                    println!("Nested async block");
                                }.await;
                            }.await;
                        }.await
                    };
                    func.block.stmts.insert(0, Stmt::Expr(nested_async_block, None));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts deeply nested async blocks into existing async functions. This transformation aims to stress the compiler's async/await handling, potentially uncovering issues related to task scheduling, future resolution, or stack overflows due to excessive nesting. It challenges the compiler's ability to manage complex async control flows and resource management."
    }
}