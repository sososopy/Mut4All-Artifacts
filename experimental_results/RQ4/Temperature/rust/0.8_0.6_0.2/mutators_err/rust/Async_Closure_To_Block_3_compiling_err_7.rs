use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Async_Closure_To_Block_3;

impl Mutator for Async_Closure_To_Block_3 {
    fn name(&self) -> &str {
        "Async_Closure_To_Block_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Stmt(Stmt::Local(item_local)) = item {
                if let Some((_, expr)) = &item_local.init {
                    if let Expr::Closure(expr_closure) = &**expr {
                        if expr_closure.asyncness.is_some() && expr_closure.capture.is_some() {
                            let args = &expr_closure.inputs;
                            let body = &expr_closure.body;

                            let closure_fn: Expr = parse_quote! {
                                |#args| #body
                            };

                            let async_block: Expr = parse_quote! {
                                async {
                                    let closure_fn = #closure_fn;
                                    closure_fn(args)
                                }
                            };

                            item_local.init = Some((token::Eq::default(), Box::new(async_block)));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures using the syntax `async move |args| {}` and replaces them with equivalent async blocks. This transformation ensures compatibility with stable Rust by using async blocks and regular functions, helping to identify compiler issues related to async behavior and type inference."
    }
}