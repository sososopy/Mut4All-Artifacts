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

pub struct Replace_Lifetime_Parameters_With_Inferred_Lifetimes_194;

impl Mutator for Replace_Lifetime_Parameters_With_Inferred_Lifetimes_194 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameters_With_Inferred_Lifetimes_194"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    continue;
                }
                let generics = func.sig.generics.clone();
                let params = generics.params
                    .into_iter()
                    .filter_map(|param| {
                        if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = param {
                            None
                        } else {
                            Some(param)
                        }
                    })
                    .collect();
                let new_generics = syn::Generics {
                    params,
                    where_clause: generics.where_clause,
                    lt_token: generics.lt_token,
                    gt_token: generics.gt_token,
                    where_clause: generics.where_clause,
                };
                func.sig.generics = new_generics;
            }
            if let syn::Item::Impl(impl_item) = item {
                if impl_item.generics.params.is_empty() {
                    continue;
                }
                let generics = impl_item.generics.clone();
                let params = generics.params
                    .into_iter()
                    .filter_map(|param| {
                        if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = param {
                            None
                        } else {
                            Some(param)
                        }
                    })
                    .collect();
                let new_generics = syn::Generics {
                    params,
                    where_clause: generics.where_clause,
                    lt_token: generics.lt_token,
                    gt_token: generics.gt_token,
                    where_clause: generics.where_clause,
                };
                impl_item.generics = new_generics;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces explicit lifetime parameters with inferred lifetimes in function and impl signatures. This transformation aims to test the compiler's handling of lifetime inference and elision, potentially triggering ICEs or inference failures related to lifetime resolution."
    }
}