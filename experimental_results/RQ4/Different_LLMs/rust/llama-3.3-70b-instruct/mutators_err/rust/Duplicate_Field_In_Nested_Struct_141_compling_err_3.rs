use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
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

pub struct Duplicate_Field_In_Nested_Struct_141;

impl Mutator for Duplicate_Field_In_Nested_Struct_141 {
    fn name(&self) -> &str {
        "Duplicate_Field_In_Nested_Struct_141"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let syn::Fields::Named(fields) = &struct_item.fields {
                    for field in fields.named.iter() {
                        if let Type::Path(TypePath { path: SynPath { segments, .. }, .. }) = &field.ty {
                            if let Some(segment) = segments.last() {
                                let ident = segment.ident.clone();
                                if let Some(ident) = ident {
                                    if ident == "Struct" {
                                        if let Type::Path(TypePath { path: SynPath { segments, .. }, .. }) = &field.ty {
                                            if let Some(segment) = segments.last() {
                                                let ident = segment.ident.clone();
                                                if let Some(ident) = ident {
                                                    if ident == "Struct" {
                                                        if let syn::Fields::Named(nested_fields) = &struct_item.fields {
                                                            let field_names: Vec<_> = nested_fields.named.iter().map(|f| f.ident.clone()).collect();
                                                            if !field_names.is_empty() {
                                                                let duplicated_field_name = field_names.choose(&mut thread_rng()).unwrap();
                                                                let duplicated_field = nested_fields.named.iter().find(|f| f.ident == *duplicated_field_name).unwrap();
                                                                let new_field = syn::Field {
                                                                    attrs: duplicated_field.attrs.clone(),
                                                                    vis: duplicated_field.vis.clone(),
                                                                    ident: duplicated_field.ident.clone(),
                                                                    colon_token: duplicated_field.colon_token.clone(),
                                                                    ty: duplicated_field.ty.clone(),
                                                                    mutability: duplicated_field.mutability.clone(),
                                                                };
                                                                nested_fields.named.push(new_field);
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
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested structs and unions in Rust code, aiming to expose bugs related to field uniqueness checks in the compiler. It applies to any struct or union that contains at least one nested struct or union. The operator duplicates an existing field within the nested struct or union, which will have the same name as an existing field in the same scope."
    }
}