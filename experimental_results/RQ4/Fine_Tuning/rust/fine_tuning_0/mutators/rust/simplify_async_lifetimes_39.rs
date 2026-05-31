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

pub struct Simplify_Async_Lifetimes_39;

impl Mutator for Simplify_Async_Lifetimes_39 {
    fn name(&self) -> &str {
        "Simplify_Async_Lifetimes_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let mut lifetimes: Vec<&Lifetime> = vec![];
                    for param in &func.sig.generics.params {
                        if let GenericParam::Lifetime(lifetime_def) = param {
                            lifetimes.push(&lifetime_def.lifetime);
                        }
                    }
                    if lifetimes.len() > 1 {
                        let first_lifetime = lifetimes[0].clone();
                        func.sig.generics.params = func.sig.generics.params.iter().filter_map(|param| {
                            if let GenericParam::Lifetime(_) = param {
                                None
                            } else {
                                Some(param.clone())
                            }
                        }).collect();
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: vec![],
                            lifetime: first_lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                    type_ref.lifetime = Some(first_lifetime.clone());
                                }
                            }
                        }
                        if let ReturnType::Type(_, ty) = &mut func.sig.output {
                            if let Type::Reference(type_ref) = &mut **ty {
                                type_ref.lifetime = Some(first_lifetime.clone());
                            }
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