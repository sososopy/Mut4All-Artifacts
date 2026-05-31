use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut, *,
    Item, Meta, MetaList,
};

use crate::mutator::Mutator;

pub struct Alter_Enum_Representation_18;

impl Mutator for Alter_Enum_Representation_18 {
    fn name(&self) -> &str {
        "Alter_Enum_Representation_18"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_repr = false;
                for attr in &item_enum.attrs {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            meta_list.nested.iter().for_each(|nested_meta| {
                                if let NestedMeta::Meta(Meta::Path(path)) = nested_meta {
                                    if path.is_ident("u8") || path.is_ident("u16") || path.is_ident("u32") || path.is_ident("C") || path.is_ident("packed") {
                                        has_repr = true;
                                    }
                                }
                            });
                        }
                    }
                }
                if has_repr {
                    let repr_options = ["C", "packed", "u8", "u16", "u32"];
                    let new_repr = repr_options.choose(&mut thread_rng()).unwrap();
                    item_enum.attrs.retain(|attr| !attr.path().is_ident("repr"));
                    item_enum.attrs.push(parse_quote!(#[repr(#new_repr)]));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with explicit representation attributes and changes the representation type to another valid option. This transformation can expose issues related to memory layout and ABI compatibility by altering the expected memory layout of the enum, potentially leading to compiler bugs or runtime errors."
    }
}