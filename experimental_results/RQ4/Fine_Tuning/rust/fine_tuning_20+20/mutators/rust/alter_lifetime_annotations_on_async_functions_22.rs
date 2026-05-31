use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Alter_Lifetime_Annotations_On_Async_Functions_22;

impl Mutator for Alter_Lifetime_Annotations_On_Async_Functions_22 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_On_Async_Functions_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut lifetime_count = 0;
                    item_fn.sig.generics.params = item_fn
                        .sig
                        .generics
                        .params
                        .clone()
                        .into_iter()
                        .filter(|param| {
                            if let GenericParam::Lifetime(_) = param {
                                lifetime_count += 1;
                                lifetime_count == 1
                            } else {
                                true
                            }
                        })
                        .collect();
                    if lifetime_count == 1 {
                        item_fn.sig.generics.params = item_fn
                            .sig
                            .generics
                            .params
                            .clone()
                            .into_iter()
                            .filter(|param| !matches!(param, GenericParam::Lifetime(_)))
                            .collect();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions with multiple lifetime annotations by removing all but one lifetime parameter from the function signature. If only one lifetime remains, it removes that as well, simplifying the signature to challenge the compiler's lifetime inference in async contexts."
    }
}