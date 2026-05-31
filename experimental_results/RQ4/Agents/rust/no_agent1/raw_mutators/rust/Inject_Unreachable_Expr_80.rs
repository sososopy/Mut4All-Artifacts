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

pub struct Inject_Unreachable_Expr_80;

impl Mutator for Inject_Unreachable_Expr_80 {
    fn name(&self) -> &str {
        "Inject_Unreachable_Expr_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                item_fn.block.stmts.push(Stmt::Expr(unreachable_expr, None));
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        method.block.stmts.push(Stmt::Expr(unreachable_expr, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an `unreachable!()` expression to the end of non-main function and method bodies. This transformation introduces a guaranteed panic point, testing the compiler's handling of unreachable code paths and potentially triggering ICEs related to control flow analysis or optimization passes that assume code execution never reaches this point."
    }
}