use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Ident, Item, ItemFn, parse_quote, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct ReplaceAsyncClosureWithAsyncBlock3;

impl Mutator for ReplaceAsyncClosureWithAsyncBlock3 {
    fn name(&self) -> &str {
        "ReplaceAsyncClosureWithAsyncBlock3"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = AsyncClosureVisitor;
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator replaces async closures with async blocks to test the compiler's handling of async constructs. By transforming unstable async closures into stable async blocks, it helps identify issues in async code processing."
    }
}

struct AsyncClosureVisitor;

impl VisitMut for AsyncClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(expr_closure) = expr {
            if expr_closure.asyncness.is_some() {
                let params: Vec<_> = expr_closure.inputs.iter().collect();
                let body = &expr_closure.body;
                let async_block: Expr = parse_quote! {
                    {
                        #(let #params = #params;)*
                        async move { #body }
                    }
                };
                *expr = async_block;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}