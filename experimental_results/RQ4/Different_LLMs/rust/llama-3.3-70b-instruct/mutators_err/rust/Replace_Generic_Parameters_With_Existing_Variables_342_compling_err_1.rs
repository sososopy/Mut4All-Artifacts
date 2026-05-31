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

pub struct Replace_Generic_Parameters_With_Existing_Variables_342;

impl Mutator for Replace_Generic_Parameters_With_Existing_Variables_342 {
    fn name(&self) -> &str {
        "Replace_Generic_Parameters_With_Existing_Variables_342"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    let existing_variables: Vec<Ident> = struct_item
                        .fields
                        .iter()
                        .filter_map(|field| match field {
                            Field::Named(field) => Some(field.ident.clone()),
                            _ => None,
                        })
                        .collect();
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(existing_variable) = existing_variables.iter().choose(&mut thread_rng()) {
                                type_param.ident = existing_variable.clone();
                            }
                        }
                    }
                }
            } else if let Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    let existing_variables: Vec<Ident> = func
                        .sig
                        .inputs
                        .iter()
                        .filter_map(|input| match input {
                            FnArg::Typed(PatType { pat, .. }) => match pat {
                                Pat::Ident(ident) => Some(ident.ident.clone()),
                                _ => None,
                            },
                            _ => None,
                        })
                        .collect();
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(existing_variable) = existing_variables.iter().choose(&mut thread_rng()) {
                                type_param.ident = existing_variable.clone();
                            }
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    let existing_variables: Vec<Ident> = impl_item
                        .items
                        .iter()
                        .filter_map(|item| match item {
                            ImplItem::Fn(func) => func
                                .sig
                                .inputs
                                .iter()
                                .filter_map(|input| match input {
                                    FnArg::Typed(PatType { pat, .. }) => match pat {
                                        Pat::Ident(ident) => Some(ident.ident.clone()),
                                        _ => None,
                                    },
                                    _ => None,
                                })
                                .next(),
                            _ => None,
                        })
                        .collect();
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(existing_variable) = existing_variables.iter().choose(&mut thread_rng()) {
                                type_param.ident = existing_variable.clone();
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic parameter names with existing variable names in the same scope. This transformation aims to test the compiler's handling of name conflicts and scope resolution issues in generic types."
    }
}