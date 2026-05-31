use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprClosure, File, Item, ItemFn, Stmt, parse_quote, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Introduce_Nested_Closures_27;

impl Mutator for Introduce_Nested_Closures_27 {
    fn name(&self) -> &str {
        "Introduce_Nested_Closures_27"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(closure) = func.block.stmts.iter_mut().find_map(|stmt| {
                    if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                        Some(closure)
                    } else {
                        None
                    }
                }) {
                    let nested_closure: Expr = parse_quote! {
                        || 0
                    };

                    let nested_closure_stmt: Stmt = parse_quote! {
                        let inner_closure = #nested_closure;
                    };

                    let invoke_nested_closure: Stmt = parse_quote! {
                        inner_closure();
                    };

                    closure.body = Box::new(parse_quote!({
                        #nested_closure_stmt
                        #invoke_nested_closure
                    }));
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a nested closure inside existing closure expressions found in the code. By nesting closures, this transformation aims to stress test the Rust compiler's capabilities in handling complex closure scenarios, particularly in terms of type inference and specialization when advanced features like `type_alias_impl_trait` are in use. The nested closure is defined and immediately invoked within the outer closure to maintain valid syntax and semantics."
    }
}