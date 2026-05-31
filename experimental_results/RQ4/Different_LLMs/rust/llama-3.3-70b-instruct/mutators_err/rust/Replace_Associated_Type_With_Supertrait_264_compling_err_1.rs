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

pub struct Replace_Associated_Type_With_Supertrait_264;

impl Mutator for Replace_Associated_Type_With_Supertrait_264 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_With_Supertrait_264"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(item_type) = item {
                        if let Some(supertrait) = find_supertrait(&trait_item.supertraits) {
                            let supertrait_path = supertrait.path.clone();
                            let supertrait_associated_type = find_associated_type(&supertrait_path, &trait_item.items);
                            if let Some(associated_type) = supertrait_associated_type {
                                item_type.default = Some(associated_type.clone());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces associated types in traits with their supertrait associated types if available. This transformation tests the compiler's handling of trait relationships and associated types, potentially leading to ICEs or unexpected behavior in the trait system."
    }
}

fn find_supertrait(supertraits: &Punctuated<TraitBound, Comma>) -> Option<&TraitBound> {
    for supertrait in supertraits {
        if let Type::Path(path) = &*supertrait.path {
            if path.segments.len() == 1 {
                return Some(supertrait);
            }
        }
    }
    None
}

fn find_associated_type(path: &SynPath, items: &Punctuated<TraitItem, Comma>) -> Option<Type> {
    for item in items {
        if let TraitItem::Type(item_type) = item {
            if let Some(segment) = path.segments.last() {
                if segment.ident == item_type.ident {
                    return item_type.default.clone();
                }
            }
        }
    }
    None
}