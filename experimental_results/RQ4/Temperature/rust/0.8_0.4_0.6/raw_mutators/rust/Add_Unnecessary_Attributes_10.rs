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

pub struct Add_Unnecessary_Attributes_10;

impl Mutator for Add_Unnecessary_Attributes_10 {
    fn name(&self) -> &str {
        "Add_Unnecessary_Attributes_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let attrs = &mut func.attrs;
                    attrs.push(parse_quote!(#[inline(always)]));
                    attrs.push(parse_quote!(#[allow(dead_code)]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unnecessary attributes `#[inline(always)]` and `#[allow(dead_code)]` to non-main function declarations. This transformation aims to test the compiler's handling of redundant attributes and explore potential paths leading to internal compiler errors or unexpected behavior in attribute processing."
    }
}