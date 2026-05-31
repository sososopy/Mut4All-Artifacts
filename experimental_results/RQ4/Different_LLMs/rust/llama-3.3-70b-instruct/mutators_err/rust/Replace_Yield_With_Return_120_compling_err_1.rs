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

pub struct Replace_Yield_With_Return_120;

impl Mutator for Replace_Yield_With_Return_120 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Return_120"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_block = func.block.clone();
                new_block.stmts = new_block.stmts.into_iter().map(|stmt| {
                    if let Stmt::Expr(Expr::Yield(yield_expr, _)) = stmt {
                        Stmt::Expr(Expr::Return(Some(yield_expr.expr.clone())), None)
                    } else {
                        stmt
                    }
                }).collect();
                func.block = new_block;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that use the yield keyword, typically found in generator functions or coroutines. It replaces the yield statement with a return statement, potentially altering the control flow and return type of the function."
    }
}