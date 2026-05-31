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

pub struct Modify_Enum_Variant_To_Use_PhantomData_180;

impl Mutator for Modify_Enum_Variant_To_Use_PhantomData_180 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_To_Use_PhantomData_180"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut variant_indices = Vec::new();
                for (index, variant) in item_enum.variants.iter().enumerate() {
                    if !variant.fields.is_empty() {
                        variant_indices.push(index);
                    }
                }
                if variant_indices.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let selected_index = variant_indices.choose(&mut rng).unwrap();
                let selected_variant = &mut item_enum.variants[*selected_index];
                let type_candidates: Vec<_> = file
                    .items
                    .iter()
                    .filter_map(|item| match item {
                        syn::Item::Struct(item_struct) => {
                            Some(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(item_struct.ident.clone()),
                            }))
                        }
                        syn::Item::Enum(item_enum) => Some(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path::from(item_enum.ident.clone()),
                        })),
                        syn::Item::Type(item_type) => Some((*item_type.ty).clone()),
                        _ => None,
                    })
                    .collect();
                let phantom_type = if type_candidates.is_empty() {
                    syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: syn::Path::from(syn::Ident::new("usize", Span::call_site())),
                    })
                } else {
                    type_candidates.choose(&mut rng).unwrap().clone()
                };
                if let syn::Fields::Unnamed(fields_unnamed) = &mut selected_variant.fields {
                    fields_unnamed.unnamed.clear();
                    fields_unnamed
                        .unnamed
                        .push(parse_quote!(std::marker::PhantomData<#phantom_type>));
                } else if let syn::Fields::Named(fields_named) = &mut selected_variant.fields {
                    fields_named
                        .named
                        .push(parse_quote!(phantom: std::marker::PhantomData<#phantom_type>));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with variants that have fields. It selects one such variant and modifies it to include a `PhantomData` field. For tuple-like variants, it replaces the payload with `PhantomData` of a type inferred from the context, such as existing struct or type aliases in the file. For struct-like variants, it adds a `PhantomData` field. This transformation leverages Rust's zero-sized type mechanism to introduce subtle layout changes, potentially exposing compiler bugs related to data layout, type inference, and trait implementations."
    }
}