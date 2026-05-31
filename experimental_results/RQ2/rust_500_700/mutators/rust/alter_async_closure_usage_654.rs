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

pub struct Alter_Async_Closure_Usage_654;

impl Mutator for Alter_Async_Closure_Usage_654 {
    fn name(&self) -> &str {
        "Alter_Async_Closure_Usage_654"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                            if path.segments.last().unwrap().ident == "block_on" {
                                if let Some(Expr::Closure(expr_closure)) = expr_call.args.first() {
                                    if expr_closure.asyncness.is_some() {
                                        let new_closure: Expr = parse_quote! {
                                            async |x: u32| {
                                                if x > 0 {
                                                    empty().await;
                                                }
                                            }
                                        };
                                        expr_call.args[0] = new_closure;
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
        "This mutation operator modifies async closures used within function parameters, specifically targeting closures that are awaited. By altering the closure's parameterization and logic, it explores different contexts and configurations of async closures, potentially revealing edge cases in async function interactions and runtime behavior."
    }
}