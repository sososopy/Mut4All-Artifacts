use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ItemFn, ItemImpl, ImplItem, Stmt, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Add_Infinite_Recursion_209;

impl Mutator for Add_Infinite_Recursion_209 {
    fn name(&self) -> &str {
        "Add_Infinite_Recursion_209"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_call: Expr = parse_quote! { #func.sig.ident() };
                func.block.stmts.push(Stmt::Expr(recursive_call, None));
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let recursive_call: Expr = parse_quote! { #func.sig.ident() };
                        func.block.stmts.push(Stmt::Expr(recursive_call, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an infinite recursive call to each non-main function and method. This transformation is designed to test the compiler's handling of recursion limits and stack overflow scenarios, potentially triggering ICEs or hangs due to unbounded recursion."
    }
}