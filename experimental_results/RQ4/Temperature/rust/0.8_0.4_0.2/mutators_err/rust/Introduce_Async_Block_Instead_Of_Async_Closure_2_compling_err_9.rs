use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ItemFn, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Introduce_Async_Block_Instead_Of_Async_Closure_2;

impl Mutator for Introduce_Async_Block_Instead_Of_Async_Closure_2 {
    fn name(&self) -> &str {
        "Introduce_Async_Block_Instead_Of_Async_Closure_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = AsyncClosureVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct AsyncClosureVisitor;

impl VisitMut for AsyncClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(expr_closure) = expr {
            if expr_closure.asyncness.is_some() {
                let block = &expr_closure.body;
                let inputs = &expr_closure.inputs;
                
                // Create a new async block
                let new_block: Expr = parse_quote! {
                    async move {
                        #(let _ = #inputs;)* 
                        #block
                    }
                };
                
                *expr = new_block;
            }
        }
        // Continue visiting other expressions
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}