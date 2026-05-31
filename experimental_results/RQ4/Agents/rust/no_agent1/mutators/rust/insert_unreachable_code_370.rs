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

pub struct Insert_Unreachable_Code_370;

impl Mutator for Insert_Unreachable_Code_370 {
    fn name(&self) -> &str {
        "Insert_Unreachable_Code_370"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                let mut new_block = func.block.clone();
                new_block.stmts.insert(0, Stmt::Expr(unreachable_expr, None));
                func.block = new_block;
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        let mut new_block = func.block.clone();
                        new_block.stmts.insert(0, Stmt::Expr(unreachable_expr, None));
                        func.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts `unreachable!()` at the beginning of non-main function and method bodies. This transformation introduces guaranteed unreachable code paths, potentially triggering compiler optimizations or analysis that mishandle unreachable code scenarios. It aims to stress the compiler's ability to deal with unreachable code and may expose bugs in control flow analysis or optimization phases."
    }
}