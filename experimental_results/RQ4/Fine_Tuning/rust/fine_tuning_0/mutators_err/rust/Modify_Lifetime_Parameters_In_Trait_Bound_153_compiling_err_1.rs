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

pub struct Modify_Lifetime_Parameters_In_Trait_Bound_153;

impl Mutator for Modify_Lifetime_Parameters_In_Trait_Bound_153 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Trait_Bound_153"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.lifetimes().count() == 2 {
                    let mut lifetimes: Vec<_> = func.sig.generics.lifetimes().collect();
                    if lifetimes.len() == 2 {
                        lifetimes.swap(0, 1);
                        func.sig.generics.params = lifetimes
                            .into_iter()
                            .map(|lt| GenericParam::Lifetime(lt.clone()))
                            .collect();

                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                                    for bound in &mut type_impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                                lifetimes.lifetimes.swap(0, 1);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                for bound in &mut type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                            lifetimes.lifetimes.swap(0, 1);
                                        }
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
        "The mutation operator swaps lifetime parameters in function signatures with trait bounds. It targets functions with exactly two lifetime parameters and reverses their order in both parameter and return type trait bounds. This transformation aims to test the compiler's ability to handle lifetime parameter swaps, potentially leading to lifetime mismatches and region errors."
    }
}