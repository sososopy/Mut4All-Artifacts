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

pub struct Modify_Async_Block_In_NoMangle_10;

impl Mutator for Modify_Async_Block_In_NoMangle_10 {
    fn name(&self) -> &str {
        "Modify_Async_Block_In_NoMangle_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.attrs.iter().any(|attr| attr.path().is_ident("no_mangle")) {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &mut local.init {
                                if let Expr::Call(ExprCall { func, args, .. }) = &mut *init.expr {
                                    if let Expr::Path(ExprPath { path, .. }) = &**func {
                                        if path.segments.iter().any(|seg| seg.ident == "block_on") {
                                            let mut new_block = parse_quote! {
                                                async move {
                                                    println!("Executing async block");
                                                    42
                                                }
                                            };
                                            if let Some(arg) = args.first_mut() {
                                                if let Expr::Block(expr_block) = arg {
                                                    if expr_block.block.stmts.is_empty() {
                                                        expr_block.block.stmts.push(Stmt::Expr(parse_quote! {
                                                            println!("Executing async block")
                                                        }));
                                                        expr_block.block.stmts.push(Stmt::Expr(parse_quote! {
                                                            42
                                                        }));
                                                    }
                                                    new_block = expr_block.clone();
                                                }
                                            }
                                            if let Some(arg) = args.first_mut() {
                                                *arg = Expr::Block(new_block);
                                            }
                                            *func = Box::new(parse_quote! { runtime.block_on });
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
        "The mutation operator targets `#[no_mangle]` functions containing async block executions. It modifies the async block to include a print statement and a return value if empty, and switches the execution method to `runtime.block_on`. This aims to test coroutine handling and monomorphisation in the presence of `#[no_mangle]`."
    }
}