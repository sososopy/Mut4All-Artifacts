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

pub struct Replace_Trait_Bound_311;

impl Mutator for Replace_Trait_Bound_311 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_311"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let trait_bounds = vec!["Send", "Sync", "Copy", "Clone", "Debug", "PartialEq"];
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(trait_bound) = get_trait_bound(type_path) {
                                let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                                replace_trait_bound(trait_bound, new_trait_bound, type_path);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(trait_bound) = get_trait_bound(type_path) {
                                        let new_trait_bound = trait_bounds.choose(&mut rng).unwrap();
                                        replace_trait_bound(trait_bound, new_trait_bound, type_path);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds in function and method signatures with random trait bounds. This transformation aims to test the compiler's handling of different trait bounds and their interactions, potentially leading to type mismatches or inference failures."
    }
}

fn get_trait_bound(type_path: &TypePath) -> Option<&TraitBound> {
    type_path.path.segments.last().and_then(|segment| {
        segment.arguments.as_path().and_then(|path| {
            path.segments.last().and_then(|segment| {
                segment.arguments.as_trait_bound()
            })
        })
    })
}

fn replace_trait_bound(trait_bound: &TraitBound, new_trait_bound: &str, type_path: &TypePath) {
    let mut new_path = type_path.path.clone();
    let mut new_segment = new_path.segments.pop().unwrap();
    let mut new_arguments = new_segment.arguments.clone();
    if let PathArguments::AngleBracketed(arguments) = &mut new_arguments {
        let mut new_bounds = Punctuated::new();
        new_bounds.push(TypeParamBound::Trait(TraitBound {
            paren_token: None,
            modifier: TraitBoundModifier::None,
            lifetimes: None,
            path: SynPath {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![PathSegment {
                    ident: Ident::new(new_trait_bound, Span::call_site()),
                    arguments: PathArguments::None,
                }]),
            },
        }));
        arguments.args = Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
            qself: None,
            path: new_path,
        }))]);
        arguments.bounds = new_bounds;
    }
    new_segment.arguments = new_arguments;
    new_path.segments.push(new_segment);
    type_path.path = new_path;
}