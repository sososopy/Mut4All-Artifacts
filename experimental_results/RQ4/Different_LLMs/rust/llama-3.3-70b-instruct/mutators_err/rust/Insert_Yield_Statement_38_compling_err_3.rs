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

pub struct Insert_Yield_Statement_38;

impl Mutator for Insert_Yield_Statement_38 {
    fn name(&self) -> &str {
        "Insert_Yield_Statement_38"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let yield_stmt = Stmt::Expr(Expr::Yield(ExprYield { expr: None }), None);
                let index = thread_rng().gen_range(0..func.block.stmts.len());
                func.block.stmts.insert(index, yield_stmt);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let yield_stmt = Stmt::Expr(Expr::Yield(ExprYield { expr: None }), None);
                        let index = thread_rng().gen_range(0..func.block.stmts.len());
                        func.block.stmts.insert(index, yield_stmt);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a yield statement at a random location within a function body, testing the compiler's handling of yield statements in various contexts, including borrowing and lifetime rules."
    }
}