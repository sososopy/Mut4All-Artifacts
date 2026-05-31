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

pub struct Incorrect_Lifetime_Specification_271;

impl Mutator for Incorrect_Lifetime_Specification_271 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Specification_271"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.sig.generics.params.is_empty() {
                    let mut has_lifetime = false;
                    for param in &func.sig.generics.params {
                        if let syn::GenericParam::Lifetime(_) = param {
                            has_lifetime = true;
                            break;
                        }
                    }
                    if has_lifetime {
                        func.sig.generics.params = func
                            .sig
                            .generics
                            .params
                            .clone()
                            .into_iter()
                            .filter(|param| !matches!(param, syn::GenericParam::Lifetime(_)))
                            .collect();
                        func.sig.inputs.iter_mut().for_each(|arg| {
                            if let syn::FnArg::Typed(pat_type) = arg {
                                if let syn::Type::Reference(type_reference) = &mut *pat_type.ty {
                                    type_reference.lifetime = Some(syn::parse_quote!('_));
                                }
                            }
                        });
                        if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let syn::Type::Reference(type_reference) = &mut **return_type {
                                type_reference.lifetime = Some(syn::parse_quote!('_));
                            }
                        }
                    } else {
                        func.sig
                            .generics
                            .params
                            .push(syn::parse_quote!('static));
                    }
                } else {
                    func.sig
                        .generics
                        .params
                        .push(syn::parse_quote!('static));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with explicit lifetime annotations. It replaces these lifetimes with anonymous lifetimes (`'_`), which can lead to inference ambiguities or errors in more complex functions. If no explicit lifetime is present, it adds a `'static` lifetime, potentially introducing incorrect lifetime constraints. This transformation aims to stress-test the compiler's lifetime inference and elision mechanisms."
    }
}