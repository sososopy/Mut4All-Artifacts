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

pub struct Replace_Generic_Bounds_With_Supertrait_Bounds_92;

impl Mutator for Replace_Generic_Bounds_With_Supertrait_Bounds_92 {
    fn name(&self) -> &str {
        "Replace_Generic_Bounds_With_Supertrait_Bounds_92"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let supertrait_bound = TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: punctuated::Punctuated::from_iter(vec![
                                            PathSegment {
                                                ident: Ident::new(
                                                    "core::any::Any",
                                                    proc_macro2::Span::call_site(),
                                                ),
                                                arguments: syn::PathArguments::None,
                                            },
                                        ]),
                                    },
                                };
                                *trait_bound = supertrait_bound;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let supertrait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: syn::TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: punctuated::Punctuated::from_iter(vec![
                                                    PathSegment {
                                                        ident: Ident::new(
                                                            "core::any::Any",
                                                            proc_macro2::Span::call_site(),
                                                        ),
                                                        arguments: syn::PathArguments::None,
                                                    },
                                                ]),
                                            },
                                        };
                                        *trait_bound = supertrait_bound;
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
        "The mutation operator replaces generic bounds with supertrait bounds. It targets generic types with trait bounds and replaces them with supertrait bounds, such as `core::any::Any`. This transformation aims to test the compiler's ability to handle complex trait bounds and const generics."
    }
}