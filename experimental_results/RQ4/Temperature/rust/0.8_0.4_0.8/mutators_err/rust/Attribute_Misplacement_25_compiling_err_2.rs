use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Attribute_Misplacement_25;

impl Mutator for Attribute_Misplacement_25 {
    fn name(&self) -> &str {
        "Attribute_Misplacement_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            let attr: syn::Attribute = parse_quote!(#[inline]);
                            local.attrs.push(attr);
                        }
                    } else if let Stmt::Expr(expr, _) = stmt {
                        let attr: syn::Attribute = parse_quote!(#[allow(dead_code)]);
                        expr.attrs.push(attr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets variable assignments and arithmetic expressions within function bodies. It inserts standard attributes like `#[inline]` and `#[allow(dead_code)]` before these expressions to test the parser's robustness against attribute misplacement. This transformation aims to expose potential parsing bugs by applying attributes in unconventional contexts."
    }
}