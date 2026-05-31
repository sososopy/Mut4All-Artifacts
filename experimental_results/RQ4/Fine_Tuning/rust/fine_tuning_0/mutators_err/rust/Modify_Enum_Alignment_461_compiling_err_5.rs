use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Attribute, Meta, Lit, Expr, File, Item, ItemEnum, parse_quote, spanned::Spanned, visit_mut::VisitMut,
    MetaList, MetaNameValue, NestedMeta
};

use crate::mutator::Mutator;

pub struct Modify_Enum_Alignment_461;

impl Mutator for Modify_Enum_Alignment_461 {
    fn name(&self) -> &str {
        "Modify_Enum_Alignment_461"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                let mut new_attrs = Vec::new();
                for attr in &enum_item.attrs {
                    if attr.path().is_ident("repr") {
                        let mut has_c = false;
                        let mut has_packed = false;
                        let mut packed_value = None;
                        attr.parse_args_with(|parser| {
                            parser.parse_nested_meta(|nested_meta| {
                                if let NestedMeta::Meta(Meta::Path(path)) = nested_meta {
                                    if path.is_ident("C") {
                                        has_c = true;
                                    }
                                }
                                if let NestedMeta::Meta(Meta::NameValue(MetaNameValue { path, value, .. })) = nested_meta {
                                    if path.is_ident("packed") {
                                        has_packed = true;
                                        if let Lit::Int(lit_int) = value {
                                            packed_value = Some(lit_int.base10_parse::<u32>().unwrap());
                                        }
                                    }
                                }
                                Ok(())
                            })
                        }).unwrap();
                        if has_c && has_packed {
                            let mut rng = thread_rng();
                            let new_alignment = if rng.gen_bool(0.5) {
                                None
                            } else {
                                Some((rng.gen_range(1..=8) as u32).next_power_of_two())
                            };
                            let new_attr = match new_alignment {
                                None => parse_quote!(#[repr(C)]),
                                Some(align) => parse_quote!(#[repr(C, align(#align))]),
                            };
                            new_attrs.push(new_attr);
                            continue;
                        }
                    }
                    new_attrs.push(attr.clone());
                }
                enum_item.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies enums with `#[repr(C, packed(N))]` attributes, changing or removing the `packed(N)` specification. It tests how different alignment strategies affect ABI and layout, potentially uncovering compiler bugs related to alignment handling."
    }
}