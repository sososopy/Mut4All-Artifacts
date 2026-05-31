use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Modify_Generic_Bounds_370;

impl Mutator for Modify_Generic_Bounds_370 {
    fn name(&self) -> &str {
        "Modify_Generic_Bounds_370"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let bounds = &mut type_param.bounds;
                        if bounds.is_empty() {
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Send", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        } else {
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Sync", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                let bounds = &mut type_param.bounds;
                                if bounds.is_empty() {
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("Send", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }));
                                } else {
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("Sync", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }));
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let bounds = &mut type_param.bounds;
                        if bounds.is_empty() {
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Send", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        } else {
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("Sync", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the generic bounds of functions, impls, and structs by adding Send or Sync trait bounds. This transformation aims to test the compiler's handling of trait bounds and may lead to inference failures or ICEs in the trait resolution system."
    }
}