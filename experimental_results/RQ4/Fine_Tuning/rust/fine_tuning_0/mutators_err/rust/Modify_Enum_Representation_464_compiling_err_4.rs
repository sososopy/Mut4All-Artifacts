use proc_macro2::{Span, TokenStream, TokenTree};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, Attribute, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemEnum, ItemFn, ItemStruct,
    Lifetime, LifetimeParam, Local, Meta, MetaList, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, visit::Visit, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Enum_Representation_464;

impl Mutator for Modify_Enum_Representation_464 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut repr_packed = false;
                let mut repr_align = false;

                // Check for repr attributes
                item_enum.attrs.retain(|attr| {
                    if let Some(segment) = attr.path().segments.first() {
                        if segment.ident == "repr" {
                            if let Meta::List(meta_list) = attr.parse_meta().ok() {
                                for nested in meta_list.nested.iter() {
                                    if let syn::NestedMeta::Meta(Meta::Path(path)) = nested {
                                        if let Some(ident) = path.get_ident() {
                                            if ident == "packed" {
                                                repr_packed = true;
                                            } else if ident.to_string().starts_with("align") {
                                                repr_align = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    true
                });

                // Apply mutation if conditions are met
                if repr_packed && repr_align {
                    // Remove align attribute
                    item_enum.attrs.retain(|attr| {
                        if let Some(segment) = attr.path().segments.first() {
                            if segment.ident == "repr" {
                                if let Meta::List(meta_list) = attr.parse_meta().ok() {
                                    let mut new_nested = Punctuated::new();
                                    for nested in meta_list.nested.iter() {
                                        if let syn::NestedMeta::Meta(Meta::Path(path)) = nested {
                                            if let Some(ident) = path.get_ident() {
                                                if !ident.to_string().starts_with("align") {
                                                    new_nested.push(nested.clone());
                                                }
                                            }
                                        }
                                    }
                                    let new_meta = Meta::List(MetaList {
                                        path: meta_list.path.clone(),
                                        delimiter: meta_list.delimiter,
                                        nested: new_nested,
                                    });
                                    *attr = Attribute {
                                        pound_token: attr.pound_token,
                                        style: attr.style,
                                        bracket_token: attr.bracket_token,
                                        path: attr.path.clone(),
                                        tokens: new_meta.into_token_stream(),
                                    };
                                }
                            }
                        }
                        true
                    });

                    // Add a new variant with fields of different sizes
                    item_enum.variants.push(parse_quote! {
                        VariantWithFields { a: u16, b: u32 }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies enums with `#[repr(packed, align(...))]` attributes, removes the `align` attribute, and adds a new variant with fields of varying sizes. This transformation tests the compiler's handling of enums with complex memory layouts and representation changes, particularly in transmutation contexts."
    }
}