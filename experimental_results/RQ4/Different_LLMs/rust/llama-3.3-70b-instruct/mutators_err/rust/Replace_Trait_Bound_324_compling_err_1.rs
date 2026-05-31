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

pub struct Replace_Trait_Bound_324;

impl Mutator for Replace_Trait_Bound_324 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_324"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut available_traits: HashSet<syn::Path> = HashSet::new();

        // Collect available traits in the file
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                available_traits.insert(trait_item.ident.clone().into());
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for trait_bound in &mut impl_item.trait_ {
                    if let Some(new_trait) = available_traits.iter().choose(&mut thread_rng()) {
                        *trait_bound = syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: new_trait.clone(),
                        };
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            for bound in &mut type_param.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(new_trait) = available_traits.iter().choose(&mut thread_rng()) {
                                        *trait_bound = syn::TraitBound {
                                            paren_token: None,
                                            modifier: syn::TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: new_trait.clone(),
                                        };
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
        "The mutation operator replaces trait bounds in the code with other available traits in the same scope. It targets trait implementations and trait definitions where a trait is bounded by another trait. This transformation helps test the Rust compiler's handling of different trait bounds and their interactions."
    }
}