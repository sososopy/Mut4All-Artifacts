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

pub struct Replace_FnOnce_Bound_With_AsyncFnOnce_496;

impl Mutator for Replace_FnOnce_Bound_With_AsyncFnOnce_496 {
    fn name(&self) -> &str {
        "Replace_FnOnce_Bound_With_AsyncFnOnce_496"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        if let Some(colon_token) = &type_param.colon_token {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_param.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("FnOnce") {
                                        new_bounds.push(syn::TypeParamBound::Trait(
                                            syn::TraitBound {
                                                paren_token: trait_bound.paren_token.clone(),
                                                modifier: trait_bound.modifier.clone(),
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: syn::Path {
                                                    leading_colon: trait_bound
                                                        .path
                                                        .leading_colon
                                                        .clone(),
                                                    segments: {
                                                        let mut segs = Punctuated::new();
                                                        segs.push(syn::PathSegment {
                                                            ident: Ident::new(
                                                                "AsyncFnOnce",
                                                                Span::call_site(),
                                                            ),
                                                            arguments: syn::PathArguments::None,
                                                        });
                                                        segs
                                                    },
                                                },
                                            },
                                        ));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            type_param.bounds = new_bounds;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with generic parameters bounded by the `FnOnce` trait and replaces the `FnOnce` bound with `AsyncFnOnce`. This transformation alters the expected execution context from synchronous to asynchronous, potentially exposing issues in the compiler's handling of trait resolution, async trait support, and context-specific behavior, especially in scenarios involving closures and async functions."
    }
}