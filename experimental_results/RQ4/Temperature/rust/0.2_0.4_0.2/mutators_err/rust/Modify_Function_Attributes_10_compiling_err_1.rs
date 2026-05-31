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

pub struct Modify_Function_Attributes_10;

impl Mutator for Modify_Function_Attributes_10 {
    fn name(&self) -> &str {
        "Modify_Function_Attributes_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_no_mangle = false;
                func.attrs.retain(|attr| {
                    if attr.path.is_ident("no_mangle") {
                        has_no_mangle = true;
                        false
                    } else {
                        true
                    }
                });
                if has_no_mangle {
                    // Randomly decide to either remove the attribute or replace it
                    if thread_rng().gen_bool(0.5) {
                        // Replace with another attribute
                        func.attrs.push(parse_quote!(#[inline]));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with the `#[no_mangle]` attribute. It either removes this attribute or replaces it with `#[inline]`, testing the compiler's handling of function linkage and name mangling. This transformation aims to uncover potential issues in scenarios where function visibility and linkage are critical, such as in FFI contexts."
    }
}