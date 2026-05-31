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

pub struct Inject_Unreachable_Expression_127;

impl Mutator for Inject_Unreachable_Expression_127 {
    fn name(&self) -> &str {
        "Inject_Unreachable_Expression_127"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                let mut new_block = item_fn.block.clone();
                new_block.stmts.push(Stmt::Expr(unreachable_expr));
                item_fn.block = new_block;
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        let mut new_block = method.block.clone();
                        new_block.stmts.push(Stmt::Expr(unreachable_expr));
                        method.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator injects an `unreachable!()` expression at the end of function and method bodies, excluding the main function. This transformation introduces a guaranteed panic point, testing the compiler's handling of unreachable code and panic propagation. It aims to uncover bugs related to control flow analysis and optimization around unreachable expressions."
    }
}