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

pub struct Add_Unreachable_Code_51;

impl Mutator for Add_Unreachable_Code_51 {
    fn name(&self) -> &str {
        "Add_Unreachable_Code_51"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                let mut new_block = func.block.clone();
                new_block.stmts.push(Stmt::Expr(unreachable_expr, None));
                func.block = new_block;
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        let mut new_block = func.block.clone();
                        new_block.stmts.push(Stmt::Expr(unreachable_expr, None));
                        func.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an `unreachable!()` macro call to the end of non-main function and method bodies. This transformation introduces unreachable code paths, testing the compiler's dead code analysis, control flow integrity checks, and error reporting for unreachable code scenarios. It aims to provoke ICEs or assertion failures related to control flow analysis."
    }
}