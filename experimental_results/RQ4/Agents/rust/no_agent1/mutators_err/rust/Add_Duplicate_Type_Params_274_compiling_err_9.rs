use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, BoundLifetimes, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime,
    LifetimeParam, Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt,
    TraitBound, TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath,
    punctuated::Punctuated, token, token::Comma, token::Paren, token::Plus, visit::Visit,
};

use crate::mutator::Mutator;

pub struct AddDuplicateTypeParams274;

impl Mutator for AddDuplicateTypeParams274 {
    fn name(&self) -> &str {
        "AddDuplicateTypeParams274"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    let existing_params: Vec<Ident> = generics.params.iter().filter_map(|param| {
                        if let syn::GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    }).collect();

                    for ident in existing_params {
                        generics.params.push(syn::GenericParam::Type(parse_quote!(#ident)));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            let existing_params: Vec<Ident> = generics.params.iter().filter_map(|param| {
                                if let syn::GenericParam::Type(type_param) = param {
                                    Some(type_param.ident.clone())
                                } else {
                                    None
                                }
                            }).collect();

                            for ident in existing_params {
                                generics.params.push(syn::GenericParam::Type(parse_quote!(#ident)));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates existing type parameters in non-main function and impl signatures. By introducing redundant type parameters, it aims to stress the compiler's type resolution and generic handling systems, potentially triggering ICEs or other bugs related to type parameter conflicts and resolution failures."
    }
}