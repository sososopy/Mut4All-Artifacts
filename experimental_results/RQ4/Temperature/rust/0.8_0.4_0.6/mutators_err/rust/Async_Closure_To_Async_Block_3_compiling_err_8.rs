use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default::Default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut
};
use crate::mutator::Mutator;

pub struct Async_Closure_To_Async_Block_3;

impl Mutator for Async_Closure_To_Async_Block_3 {
    fn name(&self) -> &str {
        "Async_Closure_To_Async_Block_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = &*init.expr {
                                if closure.asyncness.is_some() {
                                    let inputs = &closure.inputs;
                                    let body = &closure.body;
                                    let async_block: Expr = parse_quote! {
                                        async move {
                                            let (#inputs) = #body;
                                        }
                                    };
                                    let new_local = Local {
                                        init: Some(LocalInit {
                                            eq_token: token::Eq::default(),
                                            expr: Box::new(async_block),
                                            diverge: None,
                                        }),
                                        ..local.clone()
                                    };
                                    new_stmts.push(Stmt::Local(new_local));
                                    continue;
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures and transforms them into async blocks. This transformation involves moving the closure's parameters and body into an async block, thus changing the execution context and potentially revealing differences in behavior due to the distinct handling of async blocks versus async closures by the compiler."
    }
}