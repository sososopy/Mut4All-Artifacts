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

pub struct Modify_NoMangle_Attribute_10;

impl Mutator for Modify_NoMangle_Attribute_10 {
    fn name(&self) -> &str {
        "Modify_NoMangle_Attribute_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_no_mangle = func.attrs.iter().any(|attr| attr.path.is_ident("no_mangle"));
                if has_no_mangle {
                    // Remove the #[no_mangle] attribute
                    func.attrs.retain(|attr| !attr.path.is_ident("no_mangle"));
                } else {
                    // Add the #[no_mangle] attribute
                    func.attrs.push(parse_quote!(#[no_mangle]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator modifies the usage of the #[no_mangle] attribute on functions. It either removes the attribute if present or adds it if absent. This transformation tests the compiler's handling of name mangling and linkage, potentially exposing issues related to external integration and symbol visibility."
    }
}