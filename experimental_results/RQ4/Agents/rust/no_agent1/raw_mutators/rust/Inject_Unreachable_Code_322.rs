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

pub struct Inject_Unreachable_Code_322;

impl Mutator for Inject_Unreachable_Code_322 {
    fn name(&self) -> &str {
        "Inject_Unreachable_Code_322"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                func.block.stmts.push(Stmt::Expr(unreachable_expr, None));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        method.block.stmts.push(Stmt::Expr(unreachable_expr, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an `unreachable!()` macro call to the end of non-main functions and methods. This macro call indicates that the code should never be reached, potentially causing the compiler to generate unreachable code warnings or errors. This transformation tests the compiler's handling of unreachable code paths and its ability to optimize or detect dead code, potentially leading to ICEs or assertion failures."
    }
}