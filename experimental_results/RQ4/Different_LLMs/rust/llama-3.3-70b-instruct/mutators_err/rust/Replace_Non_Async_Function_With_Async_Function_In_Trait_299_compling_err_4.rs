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

pub struct Replace_Non_Async_Function_With_Async_Function_In_Trait_299;

impl Mutator for Replace_Non_Async_Function_With_Async_Function_In_Trait_299 {
    fn name(&self) -> &str {
        "Replace_Non_Async_Function_With_Async_Function_In_Trait_299"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        if let ReturnType::Default = method.sig.output {
                            method.sig.asyncness = Some(token::Async {
                                span: Span::call_site(),
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces non-async functions with async functions in traits. This transformation introduces async functions into traits, which is an incomplete feature in Rust. By applying this mutation, we can increase the chances of triggering bugs related to async functions in traits and testing the compiler's handling of this feature."
    }
}

impl Replace_Non_Async_Function_With_Async_Function_In_Trait_299 {
    fn mutate_method(&self, method: &mut syn::TraitItem) {
        if let syn::TraitItem::Method(method) = method {
            if let ReturnType::Default = method.sig.output {
                method.sig.asyncness = Some(token::Async {
                    span: Span::call_site(),
                });
            }
        }
    }
}