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

pub struct Add_Unused_Attribute_To_Functions_20;

impl Mutator for Add_Unused_Attribute_To_Functions_20 {
    fn name(&self) -> &str {
        "Add_Unused_Attribute_To_Functions_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let unused_attribute: syn::Attribute = parse_quote!(#[must_use]);
                    func.attrs.push(unused_attribute);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused or semantically misleading attribute `#[must_use]` to non-main functions. This transformation is intended to test the compiler's handling of attributes that are syntactically correct but semantically irrelevant or incorrect, potentially provoking lint-related issues or warnings."
    }
}