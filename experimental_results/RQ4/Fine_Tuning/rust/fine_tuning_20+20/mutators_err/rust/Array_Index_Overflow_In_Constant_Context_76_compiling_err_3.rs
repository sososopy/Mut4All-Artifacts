use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, str::FromStr,
};
use syn::{
    parse_quote,
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

pub struct Array_Index_Overflow_In_Constant_Context_76;

impl Mutator for Array_Index_Overflow_In_Constant_Context_76 {
    fn name(&self) -> &str {
        "Array_Index_Overflow_In_Constant_Context_76"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Expr::Block(expr_block) = &*item_const.expr {
                    if expr_block.label.is_none()
                        && expr_block
                            .block
                            .stmts
                            .iter()
                            .any(|stmt| matches!(stmt, syn::Stmt::Item(syn::Item::Verbatim(_))))
                    {
                        for stmt in &mut expr_block.block.stmts {
                            if let syn::Stmt::Item(syn::Item::Verbatim(item_unsafe)) = stmt {
                                let mut tokens = item_unsafe.to_string();
                                let mut block = syn::parse_str::<syn::Block>(&tokens).unwrap();
                                for stmt in &mut block.stmts {
                                    if let syn::Stmt::Local(local) = stmt {
                                        if let Some(init) = &mut local.init {
                                            if let syn::Expr::Index(expr_index) = init.expr.as_mut() {
                                                expr_index.index =
                                                    Box::new(syn::Expr::Verbatim(quote! {
                                                        usize::MAX - 1
                                                    }));
                                            }
                                        }
                                    }
                                }
                                tokens = block.to_token_stream().to_string();
                                *item_unsafe = syn::Expr::Verbatim(TokenStream::from_str(&tokens).unwrap());
                            }
                        }
                    }
                }
            }
            if let syn::Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let syn::Stmt::Item(syn::Item::Verbatim(item_unsafe)) = stmt {
                            let mut tokens = item_unsafe.to_string();
                            let mut block = syn::parse_str::<syn::Block>(&tokens).unwrap();
                            for stmt in &mut block.stmts {
                                if let syn::Stmt::Local(local) = stmt {
                                    if let Some(init) = &mut local.init {
                                        if let syn::Expr::Index(expr_index) = init.expr.as_mut() {
                                            expr_index.index =
                                                Box::new(syn::Expr::Verbatim(quote! {
                                                    usize::MAX - 1
                                                }));
                                        }
                                    }
                                }
                            }
                            tokens = block.to_token_stream().to_string();
                            *item_unsafe = syn::Expr::Verbatim(TokenStream::from_str(&tokens).unwrap());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets arrays within unsafe blocks inside constant functions or constant contexts. It modifies the index used to access the array to `usize::MAX - 1`, which is an intentionally large value that can cause an overflow. This transformation is designed to stress test the Rust compiler's handling of array indexing in constant evaluation, particularly in unsafe contexts, by introducing a potential overflow scenario that may reveal vulnerabilities or limitations in the constant evaluation engine."
    }
}