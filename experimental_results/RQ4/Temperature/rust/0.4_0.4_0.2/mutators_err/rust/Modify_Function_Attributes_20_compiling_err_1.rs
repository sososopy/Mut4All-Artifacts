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

pub struct Modify_Function_Attributes_20;

impl Mutator for Modify_Function_Attributes_20 {
    fn name(&self) -> &str {
        "Modify_Function_Attributes_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                let new_attrs = vec![
                    parse_quote!(#[inline(always)]),
                    parse_quote!(#[deprecated]),
                    parse_quote!(#[allow(dead_code)]),
                ];
                func.attrs.clear();
                func.attrs.push(new_attrs.choose(&mut rng).unwrap().clone());
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies existing function attributes or introduces new ones such as #[inline(always)], #[deprecated], or #[allow(dead_code)]. This transformation aims to explore different compiler behaviors and diagnostic emissions related to function attributes, potentially revealing issues in attribute handling and linting."
    }
}