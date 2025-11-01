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

pub struct Alter_Lifetime_Annotations_7;

impl Mutator for Alter_Lifetime_Annotations_7 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() && item_fn.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_))) {
                    let mut lifetimes_to_remove = Vec::new();
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            lifetimes_to_remove.push(lifetime.clone());
                        }
                    }
                    if lifetimes_to_remove.len() > 1 {
                        item_fn.sig.generics.params = Punctuated::new();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with multiple lifetime annotations. It identifies these functions and removes all but one lifetime annotation. If only one lifetime annotation remains, it removes that as well. This challenges the compiler's ability to infer lifetimes accurately in async functions, potentially exposing weaknesses in lifetime resolution."
    }
}