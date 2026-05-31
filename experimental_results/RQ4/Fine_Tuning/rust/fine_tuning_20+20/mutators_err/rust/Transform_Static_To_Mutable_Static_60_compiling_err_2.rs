use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, token::Comma, token::Paren,
    token::Plus, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Transform_Static_To_Mutable_Static_60;

impl Mutator for Transform_Static_To_Mutable_Static_60 {
    fn name(&self) -> &str {
        "Transform_Static_To_Mutable_Static_60"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut static_vars = Vec::new();
        for item in &file.items {
            if let Item::Static(item_static) = item {
                if matches!(item_static.mutability, StaticMutability::None) {
                    static_vars.push(item_static.ident.clone());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if static_vars.contains(&item_static.ident) {
                    item_static.mutability = StaticMutability::Mut;
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    let mut new_stmts = Vec::new();
                    for stmt in &item_fn.block.stmts {
                        let mut has_static_var = false;
                        for static_var in &static_vars {
                            if stmt
                                .to_token_stream()
                                .to_string()
                                .contains(&static_var.to_string())
                            {
                                has_static_var = true;
                                break;
                            }
                        }
                        if has_static_var {
                            let unsafe_stmt: Stmt = parse_quote! {
                                unsafe { #stmt }
                            };
                            new_stmts.push(unsafe_stmt);
                        } else {
                            new_stmts.push(stmt.clone());
                        }
                    }
                    item_fn.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify and transform immutable `static` variables to `static mut`. Ensure semantic correctness by wrapping usages in an unsafe block, specifically targeting the main function for this adjustment."
    }
}