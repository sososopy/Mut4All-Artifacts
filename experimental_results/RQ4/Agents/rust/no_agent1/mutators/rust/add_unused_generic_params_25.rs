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

pub struct Add_Unused_Generic_Params_25;

impl Mutator for Add_Unused_Generic_Params_25 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    generics.params.push(parse_quote!(U: std::fmt::Debug));
                    generics.params.push(parse_quote!(V: std::cmp::PartialOrd));
                    generics.params.push(parse_quote!(W: std::marker::Sync));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            generics.params.push(parse_quote!(U: std::fmt::Debug));
                            generics.params.push(parse_quote!(V: std::cmp::PartialOrd));
                            generics.params.push(parse_quote!(W: std::marker::Sync));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic parameters with common trait bounds to non-main function and impl signatures. This transformation introduces additional complexity in generic parameter handling, potentially leading to ICEs or inference issues as the compiler attempts to resolve these parameters despite their lack of usage within the function body."
    }
}