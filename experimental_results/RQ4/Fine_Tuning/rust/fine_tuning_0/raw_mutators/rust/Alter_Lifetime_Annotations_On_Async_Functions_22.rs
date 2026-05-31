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

pub struct Alter_Lifetime_Annotations_On_Async_Functions_22;

impl Mutator for Alter_Lifetime_Annotations_On_Async_Functions_22 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_On_Async_Functions_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let lifetime_params: Vec<_> = func.sig.generics.params.iter().filter_map(|param| {
                        if let syn::GenericParam::Lifetime(lifetime) = param {
                            Some(lifetime.clone())
                        } else {
                            None
                        }
                    }).collect();

                    if lifetime_params.len() > 1 {
                        func.sig.generics.params = func.sig.generics.params.clone().into_iter().filter(|param| {
                            if let syn::GenericParam::Lifetime(_) = param {
                                false
                            } else {
                                true
                            }
                        }).collect();

                        if !lifetime_params.is_empty() {
                            func.sig.generics.params.push(syn::GenericParam::Lifetime(lifetime_params[0].clone()));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}