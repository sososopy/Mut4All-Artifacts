use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, thread_rng};
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Expr, ExprArray, ExprAwait, ExprLit, File, Item, ItemFn, Lit, LitInt, Stmt, parse_quote,
    visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Array_Size_Mutation_In_Async_2;

impl Mutator for Array_Size_Mutation_In_Async_2 {
    fn name(&self) -> &str {
        "Array_Size_Mutation_In_Async_2"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &mut local.init {
                                if let Expr::Await(ExprAwait { base, .. }) = &mut *init.expr {
                                    if let Expr::Array(ExprArray { elems, .. }) = &**base {
                                        if let Some(Expr::Lit(ExprLit {
                                            lit: Lit::Int(lit_int),
                                            ..
                                        })) = elems.last()
                                        {
                                            if lit_int.base10_parse::<u128>().unwrap_or(0)
                                                >= 0xffff_ffff_ffff_ffff
                                            {
                                                let mut rng = thread_rng();
                                                let new_size = match rng.gen_range(0..3) {
                                                    0 => rng.gen_range(1..1024),
                                                    1 => 0,
                                                    2 => -1,
                                                    _ => 0,
                                                };
                                                let new_lit: Expr = parse_quote! {
                                                    [0u8; #new_size]
                                                };
                                                *init.expr = Expr::Await(ExprAwait {
                                                    attrs: Vec::new(),
                                                    base: Box::new(new_lit),
                                                    dot_token: token::Dot {
                                                        spans: [Span::call_site()],
                                                    },
                                                    await_token: token::Await {
                                                        span: Span::call_site(),
                                                    },
                                                });
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array initializations within asynchronous functions that use large or maximum integer values for their size. It modifies these sizes to smaller positive integers, zero, or negative values to test the Rust compiler's handling of edge cases related to integer overflow and array size in async contexts."
    }
}