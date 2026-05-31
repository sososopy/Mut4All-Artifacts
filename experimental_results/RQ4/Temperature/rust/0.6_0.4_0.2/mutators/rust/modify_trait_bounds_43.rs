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

pub struct Modify_Trait_Bounds_43;

impl Mutator for Modify_Trait_Bounds_43 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_43"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.defaultness.is_some() {
                    if let Some((_, trait_path, _)) = &item_impl.trait_ {
                        let mut new_generics = item_impl.generics.clone();
                        let mut rng = thread_rng();
                        let add_constraint = rng.gen_bool(0.5);

                        if add_constraint {
                            if let Some(first_param) = new_generics.params.first_mut() {
                                if let syn::GenericParam::Type(type_param) = first_param {
                                    type_param.bounds.push(parse_quote!(Clone));
                                }
                            }
                        } else {
                            if let Some(first_param) = new_generics.params.first_mut() {
                                if let syn::GenericParam::Type(type_param) = first_param {
                                    if !type_param.bounds.is_empty() {
                                        type_param.bounds.pop();
                                    }
                                }
                            }
                        }

                        item_impl.generics = new_generics;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with default type bounds. It randomly decides to either add a `Clone` constraint to the first generic parameter or remove an existing constraint if present. This transformation explores the compiler's handling of trait specialization and constraint management, potentially revealing issues with trait resolution and specialization mechanisms."
    }
}