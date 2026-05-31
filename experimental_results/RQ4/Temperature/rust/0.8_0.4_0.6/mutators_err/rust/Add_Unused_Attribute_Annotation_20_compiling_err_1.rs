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

pub struct Add_Unused_Attribute_Annotation_20;

impl Mutator for Add_Unused_Attribute_Annotation_20 {
    fn name(&self) -> &str {
        "Add_Unused_Attribute_Annotation_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    if !func.attrs.iter().any(|attr| attr.path.is_ident("must_use")) {
                        func.attrs.push(parse_quote!(#[must_use]));
                    }
                }
                Item::Struct(struct_item) => {
                    if !struct_item.attrs.iter().any(|attr| attr.path.is_ident("must_use")) {
                        struct_item.attrs.push(parse_quote!(#[must_use]));
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions and structs without the `#[must_use]` attribute, adding it to encourage proper usage of their results. This transformation aims to test the compiler's linting and diagnostic capabilities by ensuring that the results of these entities are utilized, potentially exposing gaps in the enforcement of usage patterns."
    }
}