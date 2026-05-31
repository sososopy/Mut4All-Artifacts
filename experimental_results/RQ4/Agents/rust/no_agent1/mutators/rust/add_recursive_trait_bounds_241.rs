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

pub struct Add_Recursive_Trait_Bounds_241;

impl Mutator for Add_Recursive_Trait_Bounds_241 {
    fn name(&self) -> &str {
        "Add_Recursive_Trait_Bounds_241"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_bound = TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("RecursiveTrait", Span::call_site()),
                                arguments: syn::PathArguments::None,
                            });
                            segs
                        },
                    },
                };
                let mut bounds = Punctuated::new();
                bounds.push(syn::TypeParamBound::Trait(recursive_bound));
                func.sig.generics.params.push(GenericParam::Type(syn::TypeParam {
                    attrs: Vec::new(),
                    ident: Ident::new("T", Span::call_site()),
                    colon_token: None,
                    bounds,
                    eq_token: None,
                    default: None,
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let recursive_bound = TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new("RecursiveTrait", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(syn::TypeParamBound::Trait(recursive_bound));
                        func.sig.generics.params.push(GenericParam::Type(syn::TypeParam {
                            attrs: Vec::new(),
                            ident: Ident::new("T", Span::call_site()),
                            colon_token: None,
                            bounds,
                            eq_token: None,
                            default: None,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive trait bound `RecursiveTrait` to function and method signatures. This transformation introduces complex trait resolution scenarios, potentially leading to infinite recursion in trait resolution or ICEs due to the compiler's handling of recursive trait bounds. It aims to test the robustness of the compiler's trait resolution system under recursive constraints."
    }
}