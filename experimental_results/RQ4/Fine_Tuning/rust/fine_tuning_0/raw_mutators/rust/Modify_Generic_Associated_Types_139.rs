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

pub struct Modify_Generic_Associated_Types_139;

impl Mutator for Modify_Generic_Associated_Types_139 {
    fn name(&self) -> &str {
        "Modify_Generic_Associated_Types_139"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = trait_item {
                        type_item.bounds.retain(|bound| {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                !trait_bound.path.segments.iter().any(|seg| {
                                    seg.ident == "F" && seg.arguments == syn::PathArguments::None
                                })
                            } else {
                                true
                            }
                        });
                        type_item.generics.where_clause = None;
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        type_item.generics.where_clause = None;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}