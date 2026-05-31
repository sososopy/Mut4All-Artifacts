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

pub struct Insert_Unreachable_Code_202;

impl Mutator for Insert_Unreachable_Code_202 {
    fn name(&self) -> &str {
        "Insert_Unreachable_Code_202"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unreachable_expr: Expr = parse_quote! { unreachable!() };
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    new_stmts.push(stmt.clone());
                    if thread_rng().gen_bool(0.5) {
                        new_stmts.push(Stmt::Expr(unreachable_expr.clone(), None));
                    }
                }
                func.block.stmts = new_stmts;
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let unreachable_expr: Expr = parse_quote! { unreachable!() };
                        let mut new_stmts = Vec::new();
                        for stmt in &func.block.stmts {
                            new_stmts.push(stmt.clone());
                            if thread_rng().gen_bool(0.5) {
                                new_stmts.push(Stmt::Expr(unreachable_expr.clone(), None));
                            }
                        }
                        func.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator randomly inserts `unreachable!()` macro calls into function and method bodies. This transformation introduces unreachable code paths, testing the compiler's handling of control flow analysis and optimization passes. It aims to uncover bugs related to dead code elimination, control flow graph construction, and panic handling in unreachable code contexts."
    }
}