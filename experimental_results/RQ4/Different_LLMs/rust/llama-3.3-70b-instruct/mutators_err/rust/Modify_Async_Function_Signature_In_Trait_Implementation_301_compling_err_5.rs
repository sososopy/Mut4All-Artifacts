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

pub struct Modify_Async_Function_Signature_In_Trait_Implementation_301;

impl Mutator for Modify_Async_Function_Signature_In_Trait_Implementation_301 {
    fn name(&self) -> &str {
        "Modify_Async_Function_Signature_In_Trait_Implementation_301"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Method(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            let lifetimes: Vec<LifetimeParam> = (0..thread_rng().gen_range(1..4))
                                .map(|_| LifetimeParam {
                                    attrs: vec![],
                                    lifetime: Lifetime::new(
                                        &format!("'a{}", thread_rng().gen::<u8>()),
                                        Span::call_site(),
                                    ),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                })
                                .collect();
                            let mut generics = func.sig.generics.clone();
                            generics.params.extend(lifetimes.into_iter().map(|lifetime| {
                                GenericParam::Lifetime(lifetime)
                            }));
                            func.sig.generics = generics;
                        }
                    }
                }
            }
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        if method.sig.asyncness.is_some() {
                            let lifetimes: Vec<LifetimeParam> = (0..thread_rng().gen_range(1..4))
                                .map(|_| LifetimeParam {
                                    attrs: vec![],
                                    lifetime: Lifetime::new(
                                        &format!("'a{}", thread_rng().gen::<u8>()),
                                        Span::call_site(),
                                    ),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                })
                                .collect();
                            let mut generics = method.sig.generics.clone();
                            generics.params.extend(lifetimes.into_iter().map(|lifetime| {
                                GenericParam::Lifetime(lifetime)
                            }));
                            method.sig.generics = generics;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the signature of async functions in trait implementations by adding, removing, or changing lifetime parameters. This transformation tests the compiler's ability to handle async functions in trait implementations and lifetime parameters, which is the root cause of the given bug report."
    }
}