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

pub struct Trait_Lifetime_Manipulation_30;

impl Mutator for Trait_Lifetime_Manipulation_30 {
    fn name(&self) -> &str {
        "Trait_Lifetime_Manipulation_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if !trait_item.generics.lifetimes().collect::<Vec<_>>().is_empty() {
                    trait_names.insert(trait_item.ident.clone());
                    trait_item.generics.params = trait_item.generics.params.iter().filter(|param| {
                        !matches!(param, syn::GenericParam::Lifetime(_))
                    }).cloned().collect();
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            type_impl_trait.bounds = type_impl_trait.bounds.iter().map(|bound| {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_names.contains(&trait_bound.path.segments.first().unwrap().ident) {
                                        let mut new_trait_bound = trait_bound.clone();
                                        new_trait_bound.lifetimes = None;
                                        return syn::TypeParamBound::Trait(new_trait_bound);
                                    }
                                }
                                bound.clone()
                            }).collect();
                        }
                    }
                }

                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        type_impl_trait.bounds = type_impl_trait.bounds.iter().map(|bound| {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_names.contains(&trait_bound.path.segments.first().unwrap().ident) {
                                    let mut new_trait_bound = trait_bound.clone();
                                    new_trait_bound.lifetimes = None;
                                    return syn::TypeParamBound::Trait(new_trait_bound);
                                }
                            }
                            bound.clone()
                        }).collect();
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        if type_impl_trait.bounds.iter().any(|bound| {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                trait_names.contains(&trait_bound.path.segments.first().unwrap().ident)
                            } else {
                                false
                            }
                        }) {
                            func.sig.generics.params.push(parse_quote!('b));
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