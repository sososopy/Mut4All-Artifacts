use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ItemFn, ItemImpl, Stmt, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Add_Infinite_Recursion_101;

impl Mutator for Add_Infinite_Recursion_101 {
    fn name(&self) -> &str {
        "Add_Infinite_Recursion_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_call: Expr = parse_quote! { #func.sig.ident() };
                let mut new_block = func.block.clone();
                new_block.stmts.push(Stmt::Expr(recursive_call));
                func.block = Box::new(new_block);
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let recursive_call: Expr = parse_quote! { #func.sig.ident() };
                        let mut new_block = func.block.clone();
                        new_block.stmts.push(Stmt::Expr(recursive_call));
                        func.block = Box::new(new_block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends a recursive call to non-main functions and methods, creating infinite recursion. This transformation aims to stress the compiler's handling of recursion limits, stack overflow detection, and potential hang scenarios. It tests the robustness of the compiler's execution and optimization strategies under extreme recursive conditions."
    }
}