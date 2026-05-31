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

pub struct Swap_Coerce_Unsized_Predicates_168;

impl Mutator for Swap_Coerce_Unsized_Predicates_168 {
    fn name(&self) -> &str {
        "Swap_Coerce_Unsized_Predicates_168"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut bounds = type_impl_trait.bounds.clone();
                        let coerce_unsized_predicates: Vec<_> = bounds
                            .iter()
                            .filter_map(|bound| {
                                if let TypeParamBound::Trait(TraitBound {
                                    path: SynPath {
                                        segments: segments,
                                        ..
                                    },
                                    ..
                                }) = bound
                                {
                                    if segments
                                        .last()
                                        .map_or(false, |segment| segment.ident == "CoerceUnsized")
                                    {
                                        Some(bound.clone())
                                    } else {
                                        None
                                    }
                                } else {
                                    None
                                }
                            })
                            .collect();
                        if coerce_unsized_predicates.len() >= 2 {
                            bounds = bounds
                                .into_iter()
                                .filter_map(|bound| {
                                    if let TypeParamBound::Trait(TraitBound {
                                        path: SynPath {
                                            segments: segments,
                                            ..
                                        },
                                        ..
                                    }) = bound
                                    {
                                        if segments
                                            .last()
                                            .map_or(false, |segment| segment.ident == "CoerceUnsized")
                                        {
                                            None
                                        } else {
                                            Some(bound)
                                        }
                                    } else {
                                        Some(bound)
                                    }
                                })
                                .collect();
                            bounds.extend(coerce_unsized_predicates.into_iter().rev());
                            type_impl_trait.bounds = bounds;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return `impl Trait`. It swaps the order of CoerceUnsized predicates in the trait bounds, potentially leading to issues with the compiler's handling of trait bounds and coercion."
    }
}