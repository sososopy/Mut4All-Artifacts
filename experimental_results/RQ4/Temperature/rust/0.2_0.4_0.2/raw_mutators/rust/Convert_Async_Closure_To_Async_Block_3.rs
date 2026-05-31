use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
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

pub struct Convert_Async_Closure_To_Async_Block_3;

impl Mutator for Convert_Async_Closure_To_Async_Block_3 {
    fn name(&self) -> &str {
        "Convert_Async_Closure_To_Async_Block_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsyncClosureVisitor;

        impl VisitMut for AsyncClosureVisitor {
            fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
                if node.asyncness.is_some() {
                    let body = &node.body;
                    let inputs = &node.inputs;

                    let mut capture_stmts: Vec<Stmt> = Vec::new();
                    let mut captured_vars: Vec<Ident> = Vec::new();

                    for input in inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                captured_vars.push(pat_ident.ident.clone());
                                capture_stmts.push(parse_quote! {
                                    let #pat_ident = #pat_ident.clone();
                                });
                            }
                        }
                    }

                    let async_block: Expr = parse_quote! {
                        {
                            #(#capture_stmts)*
                            async {
                                #body
                            }
                        }
                    };

                    node.body = Box::new(async_block);
                    node.asyncness = None;
                }
                visit_mut::visit_expr_closure_mut(self, node);
            }
        }

        let mut visitor = AsyncClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures and transforms them into async blocks. This transformation involves capturing the closure's parameters explicitly before the async block, ensuring that the async block has access to the same variables and logic as the original async closure. This mutation tests the compiler's handling of async constructs, especially given the instability of async closures."
    }
}