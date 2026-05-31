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
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Closure(closure) = &**expr {
                                if closure.asyncness.is_some() {
                                    let async_block: Expr = parse_quote! {
                                        async {
                                            let #closure.inputs = #closure.body;
                                        }
                                    };
                                    let new_local = Local {
                                        init: Some((token::Eq::default(), Box::new(async_block))),
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