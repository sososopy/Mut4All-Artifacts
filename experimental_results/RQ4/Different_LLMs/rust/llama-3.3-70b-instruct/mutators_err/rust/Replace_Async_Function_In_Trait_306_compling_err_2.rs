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

pub struct Replace_Async_Function_In_Trait_306;

impl Mutator for Replace_Async_Function_In_Trait_306 {
    fn name(&self) -> &str {
        "Replace_Async_Function_In_Trait_306"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        if method.sig.asyncness.is_some() {
                            // Replace async function with non-async function
                            method.sig.asyncness = None;
                        } else {
                            // Replace non-async function with async function
                            method.sig.asyncness = Some(Default::default());
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an async function in a trait with a non-async function and vice versa, to test how the Rust compiler handles the change in the function signature, especially when the trait is used in an async context."
    }
}