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

pub struct Modify_Const_Generic_Arguments_90;

impl Mutator for Modify_Const_Generic_Arguments_90 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Arguments_90"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(const_generic) = item_fn.sig.generics.params.iter().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    let const_ident = &const_generic.ident;
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                if path.segments.last().unwrap().ident == item_fn.sig.ident {
                                    for arg in &mut expr_call.args {
                                        if let Expr::Lit(expr_lit) = arg {
                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                if lit_int.base10_parse::<usize>().is_ok() {
                                                    *arg = parse_quote!(usize::MAX);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with const generics and modifies the const generic arguments in function calls to boundary values like `usize::MAX`. This transformation is designed to test the Rust compiler's handling of const generics by introducing edge cases that may lead to inconsistencies or internal compiler errors."
    }
}