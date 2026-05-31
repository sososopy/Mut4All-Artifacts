use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprClosure, File, Item, ItemFn, Pat, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Introduce_Nested_Closure_21;

impl Mutator for Introduce_Nested_Closure_21 {
    fn name(&self) -> &str {
        "Introduce_Nested_Closure_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ClosureVisitor::new();
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a nested closure within an existing closure inside a function. The nested closure captures a variable from the enclosing closure, adding complexity to the variable capture mechanism. This transformation stresses the compiler's handling of nested closures and variable captures, potentially uncovering bugs in these areas."
    }
}

struct ClosureVisitor;

impl ClosureVisitor {
    fn new() -> Self {
        ClosureVisitor
    }
}

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, i: &mut syn::ExprClosure) {
        if let Some(first_input) = i.inputs.first() {
            if let syn::Pat::Ident(pat_ident) = first_input {
                let var_name = &pat_ident.ident;
                let nested_closure: Expr = parse_quote! {
                    |nested_var| {
                        #var_name + nested_var
                    }
                };
                let call_nested_closure: Expr = parse_quote! {
                    nested_closure(2)
                };
                i.body = Box::new(parse_quote! {{
                    let nested_closure = #nested_closure;
                    #call_nested_closure;
                }});
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, i);
    }
}