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

pub struct Modify_Lifetime_Params_In_Assoc_Types_34;

impl Mutator for Modify_Lifetime_Params_In_Assoc_Types_34 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Params_In_Assoc_Types_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Type(assoc_type) = item {
                        if let Some(bounds) = &mut assoc_type.generics.params.first_mut() {
                            match bounds {
                                GenericParam::Lifetime(lifetime_param) => {
                                    // Decide randomly to add, remove, or swap lifetimes
                                    let mut rng = thread_rng();
                                    match rng.gen_range(0..3) {
                                        0 => { // Add an extra lifetime parameter
                                            assoc_type.generics.params.push(parse_quote!('new));
                                        }
                                        1 => { // Remove an existing lifetime parameter
                                            assoc_type.generics.params.clear();
                                        }
                                        2 => { // Swap positions of existing lifetime parameters
                                            let mut lifetimes: Vec<_> = assoc_type.generics.params.iter().collect();
                                            lifetimes.reverse();
                                            assoc_type.generics.params.clear();
                                            assoc_type.generics.params.extend(lifetimes.into_iter().cloned());
                                        }
                                        _ => {}
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if let Some(bounds) = &mut impl_type.generics.params.first_mut() {
                            match bounds {
                                GenericParam::Lifetime(lifetime_param) => {
                                    // Reflect changes in implementation
                                    let mut rng = thread_rng();
                                    match rng.gen_range(0..3) {
                                        0 => {
                                            impl_type.generics.params.push(parse_quote!('new));
                                        }
                                        1 => {
                                            impl_type.generics.params.clear();
                                        }
                                        2 => {
                                            let mut lifetimes: Vec<_> = impl_type.generics.params.iter().collect();
                                            lifetimes.reverse();
                                            impl_type.generics.params.clear();
                                            impl_type.generics.params.extend(lifetimes.into_iter().cloned());
                                        }
                                        _ => {}
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits and implementations with associated types that have lifetime parameters. It randomly adds, removes, or swaps lifetime parameters to explore the compiler's handling of lifetime variations in associated types. This aims to uncover potential issues with lifetime inference and generic constant expressions."
    }
}