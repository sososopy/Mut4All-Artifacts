use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, ExprPath, File, Item, ItemFn, ItemStatic, Local, Stmt, Type, TypePath, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Thread_Local_Static_Mutation_606;

impl Mutator for Thread_Local_Static_Mutation_606 {
    fn name(&self) -> &str {
        "Thread_Local_Static_Mutation_606"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if item_static.attrs.iter().any(|attr| attr.path().is_ident("thread_local")) {
                    if let Type::Path(TypePath { path, .. }) = &*item_static.ty {
                        if path.segments.last().map_or(false, |segment| segment.ident == "AtomicUsize") {
                            // Change type from AtomicUsize to usize
                            item_static.ty = Box::new(parse_quote!(usize));
                            // Remove initialization
                            item_static.expr = Some(Box::new(parse_quote!(0)));
                        }
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Unsafe(unsafe_block), _) = stmt {
                            for inner_stmt in &mut unsafe_block.block.stmts {
                                if let Stmt::Local(local) = inner_stmt {
                                    if let Some(LocalInit { expr, .. }) = &local.init {
                                        if let Expr::Path(ExprPath { path, .. }) = &**expr {
                                            if path.is_ident("X") {
                                                // Replace atomic operations with direct arithmetic
                                                *inner_stmt = parse_quote! {
                                                    let mut x = X;
                                                    x += 1;
                                                };
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
        "This mutation operator identifies `#[thread_local]` static variables using `AtomicUsize` and replaces them with `usize`, removing atomic operations. It modifies their usage within `unsafe` blocks to perform direct arithmetic operations instead, testing the compiler's handling of thread-local variables when atomic guarantees are removed."
    }
}