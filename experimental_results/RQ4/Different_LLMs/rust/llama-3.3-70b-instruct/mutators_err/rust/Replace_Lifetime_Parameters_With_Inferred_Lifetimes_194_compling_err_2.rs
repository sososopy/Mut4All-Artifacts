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
                if let syn::Generics::None = func.sig.generics {
                    continue;
                }
                if let syn::Generics::WhereClauses(_) = func.sig.generics {
                    continue;
                }
                let generics = func.sig.generics.take().unwrap();
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
                    syn: generics.syn,
                };
                func.sig.generics = syn::Generics::from(new_generics);
            }
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Generics::None = impl_item.generics {
                    continue;
                }
                if let syn::Generics::WhereClauses(_) = impl_item.generics {
                    continue;
                }
                let generics = impl_item.generics.take().unwrap();
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
                    syn: generics.syn,
                };
                impl_item.generics = syn::Generics::from(new_generics);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces explicit lifetime parameters with inferred lifetimes in function and impl signatures. This transformation aims to test the compiler's handling of lifetime inference and elision, potentially triggering ICEs or inference failures related to lifetime resolution."
    }
}