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

pub struct Modify_Const_Generics_In_Traits_And_Structs_105;

impl Mutator for Modify_Const_Generics_In_Traits_And_Structs_105 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Traits_And_Structs_105"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Method(method) = item {
                        if let Some((_, const_generic)) = method.sig.generics.params.iter_mut().find_map(|param| {
                            if let syn::GenericParam::Const(const_param) = param {
                                Some((&const_param.ident, param))
                            } else {
                                None
                            }
                        }) {
                            if let syn::GenericParam::Const(const_param) = const_generic {
                                const_param.ident = Ident::new("M", Span::call_site());
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                if let Some((_, const_generic)) = item_struct.generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some((&const_param.ident, param))
                    } else {
                        None
                    }
                }) {
                    if let syn::GenericParam::Const(const_param) = const_generic {
                        const_param.ident = Ident::new("M", Span::call_site());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}