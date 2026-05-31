use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Uninhabited_With_Inhabited_Field_467;

impl Mutator for Replace_Uninhabited_With_Inhabited_Field_467 {
    fn name(&self) -> &str {
        "Replace_Uninhabited_With_Inhabited_Field_467"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut empty_enum_names = HashSet::new();

        // First pass: collect all empty enums
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                if enum_item.variants.is_empty() {
                    empty_enum_names.insert(enum_item.ident.to_string());
                }
            }
        }

        // Second pass: process structs and enums
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                match &mut struct_item.fields {
                    syn::Fields::Named(fields) => {
                        for field in &mut fields.named {
                            if let Type::Path(type_path) = field.ty.as_mut() {
                                if let Some(segment) = type_path.path.segments.iter().next() {
                                    if empty_enum_names.contains(&segment.ident.to_string()) {
                                        field.ty = Box::new(parse_quote!(u8));
                                    }
                                }
                            }
                        }
                    },
                    syn::Fields::Unnamed(fields) => {
                        for field in &mut fields.unnamed {
                            if let Type::Path(type_path) = field.ty.as_mut() {
                                if let Some(segment) = type_path.path.segments.iter().next() {
                                    if empty_enum_names.contains(&segment.ident.to_string()) {
                                        field.ty = Box::new(parse_quote!(u8));
                                    }
                                }
                            }
                        }
                    },
                    syn::Fields::Unit => {}
                }
            } else if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    match &mut variant.fields {
                        syn::Fields::Named(fields) => {
                            for field in &mut fields.named {
                                if let Type::Path(type_path) = field.ty.as_mut() {
                                    if let Some(segment) = type_path.path.segments.iter().next() {
                                        if empty_enum_names.contains(&segment.ident.to_string()) {
                                            field.ty = Box::new(parse_quote!(u8));
                                        }
                                    }
                                }
                            }
                        },
                        syn::Fields::Unnamed(fields) => {
                            for field in &mut fields.unnamed {
                                if let Type::Path(type_path) = field.ty.as_mut() {
                                    if let Some(segment) = type_path.path.segments.iter().next() {
                                        if empty_enum_names.contains(&segment.ident.to_string()) {
                                            field.ty = Box::new(parse_quote!(u8));
                                        }
                                    }
                                }
                            }
                        },
                        syn::Fields::Unit => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces fields in structs or enum variants that use uninhabited types (like empty enums) with a standard inhabited type (e.g., u8). This forces the compiler to recalculate type layouts, potentially exposing bugs in transmutability checks and layout assumptions."
    }
}