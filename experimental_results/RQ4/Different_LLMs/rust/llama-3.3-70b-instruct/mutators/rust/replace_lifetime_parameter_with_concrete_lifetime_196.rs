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

pub struct Replace_Lifetime_Parameter_With_Concrete_Lifetime_196;

impl Mutator for Replace_Lifetime_Parameter_With_Concrete_Lifetime_196 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_With_Concrete_Lifetime_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime = Lifetime::new("'static", Span::call_site());
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for param in &mut impl_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime = Lifetime::new("'static", Span::call_site());
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime = Lifetime::new("'static", Span::call_site());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetime parameters with 'static in trait definitions, implementations, and function signatures. This transformation aims to test the compiler's handling of lifetime parameters and may lead to issues with lifetime handling, especially when dealing with higher-ranked nested RPIT."
    }
}