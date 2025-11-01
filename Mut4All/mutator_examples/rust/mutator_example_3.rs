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

pub struct Replace_Return_Type_With_Impl_ToString_3;

impl Mutator for Replace_Return_Type_With_Impl_ToString_3 {
    fn name(&self) -> &str {
        "Replace_Return_Type_With_Impl_ToString_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(syn::PathSegment {
                                            ident: Ident::new("ToString", Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            }));
                            bounds
                        },
                    }));
                }
            }
            if let syn::Item::Impl(tag1) = item {
                for impl_item in &mut tag1.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                                impl_token: token::Impl {
                                    span: Span::call_site(),
                                },
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segs = Punctuated::new();
                                                segs.push(syn::PathSegment {
                                                    ident: Ident::new(
                                                        "ToString",
                                                        Span::call_site(),
                                                    ),
                                                    arguments: syn::PathArguments::None,
                                                });
                                                segs
                                            },
                                        },
                                    }));
                                    bounds
                                },
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return type of non-main functions and methods with `impl ToString`. This transformation enforces a concrete opaque return constraint, potentially leading to trait bound mismatches or inference failures. It aims to test the compiler's handling of `impl Trait` returns and its ability to resolve trait implementations under altered type expectations."
    }
}
