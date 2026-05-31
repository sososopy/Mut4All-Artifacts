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

pub struct Add_Generic_Params_To_Async_Methods_300;

impl Mutator for Add_Generic_Params_To_Async_Methods_300 {
    fn name(&self) -> &str {
        "Add_Generic_Params_To_Async_Methods_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_method in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_method {
                        if method.sig.asyncness.is_some() && method.sig.generics.params.is_empty() {
                            method.sig.generics.params.push(parse_quote!(T));
                            method.sig.generics.params.push(parse_quote!(U));
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_method in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_method {
                        if method.sig.asyncness.is_some() && method.sig.generics.params.is_empty() {
                            method.sig.generics.params.push(parse_quote!(T));
                            method.sig.generics.params.push(parse_quote!(U));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces two unused generic type parameters (T and U) to async methods in traits and their implementations. This transformation stresses the compiler's handling of async generics, potentially exposing bugs in type resolution, monomorphization, or async code generation for generic functions."
    }
}