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

pub struct Mutator_ModifyEnumReprForABIAlignmentConflict_187;

impl Mutator for Mutator_ModifyEnumReprForABIAlignmentConflict_187 {
    fn name(&self) -> &str {
        "Mutator_ModifyEnumReprForABIAlignmentConflict_187"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Step 1: collect all enums and their names and indices
        let mut enum_info: Vec<(usize, String)> = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let syn::Item::Enum(e) = item {
                enum_info.push((i, e.ident.to_string()));
            }
        }

        // Step 2: collect all unions and their enum fields
        let mut enums_to_modify: Vec<(String, String)> = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Union(u) = item {
                let mut enum_names_in_union: Vec<String> = Vec::new();
                for field in &u.fields {
                    if let Type::Path(ty_path) = &field.ty {
                        if let Some(segment) = ty_path.path.segments.last() {
                            let name = segment.ident.to_string();
                            if enum_info.iter().any(|(_, en)| en == &name) {
                                enum_names_in_union.push(name);
                            }
                        }
                    }
                }
                // Check if there are at least two enums in this union
                if enum_names_in_union.len() >= 2 {
                    enums_to_modify.push((enum_names_in_union[0].clone(), enum_names_in_union[1].clone()));
                }
            }
        }

        // Step 3: for each enum to modify, apply the repr attributes
        for (enum1_name, enum2_name) in &enums_to_modify {
            for (i, en_name) in &enum_info {
                if en_name == enum1_name {
                    if let syn::Item::Enum(e) = &mut file.items[*i] {
                        // Remove existing repr attributes
                        e.attrs = e.attrs.iter()
                            .filter(|attr| !attr.path.is_ident("repr"))
                            .cloned()
                            .collect();
                        // Add new repr attribute
                        let new_attr = parse_quote! { #[repr(u32)] };
                        e.attrs.push(new_attr);
                    }
                } else if en_name == enum2_name {
                    if let syn::Item::Enum(e) = &mut file.items[*i] {
                        e.attrs = e.attrs.iter()
                            .filter(|attr| !attr.path.is_ident("repr"))
                            .cloned()
                            .collect();
                        let new_attr = parse_quote! { #[repr(C, packed(2))] };
                        e.attrs.push(new_attr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator alters the repr attributes of enums within the same union to create alignment conflicts while maintaining ABI-compatible sizes. This forces the compiler to handle differing alignment requirements for union fields, testing its ability to resolve memory layout conflicts and validate ABI compatibility under such conditions."
    }
}