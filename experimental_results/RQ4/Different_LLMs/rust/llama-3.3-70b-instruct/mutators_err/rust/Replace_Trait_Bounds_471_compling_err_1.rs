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

pub struct Replace_Trait_Bounds_471;

impl Mutator for Replace_Trait_Bounds_471 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_471"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for segment in &trait_item.path.segments {
                    existing_traits.insert(segment.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut rng = thread_rng();
                                let traits: Vec<Ident> = existing_traits.iter().cloned().collect();
                                if let Some(new_trait) = traits.choose(&mut rng) {
                                    trait_bound.path.segments[0].ident = new_trait.clone();
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut rng = thread_rng();
                                let traits: Vec<Ident> = existing_traits.iter().cloned().collect();
                                if let Some(new_trait) = traits.choose(&mut rng) {
                                    trait_bound.path.segments[0].ident = new_trait.clone();
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for param in &mut impl_item.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut rng = thread_rng();
                                let traits: Vec<Ident> = existing_traits.iter().cloned().collect();
                                if let Some(new_trait) = traits.choose(&mut rng) {
                                    trait_bound.path.segments[0].ident = new_trait.clone();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing trait bounds with new ones, potentially introducing bugs related to trait resolution. It collects all existing traits in the program, then replaces each trait bound with a randomly chosen trait from the collected set. This transformation aims to test the compiler's ability to resolve trait bounds correctly and may lead to ICEs or inference failures in the trait resolution system."
    }
}