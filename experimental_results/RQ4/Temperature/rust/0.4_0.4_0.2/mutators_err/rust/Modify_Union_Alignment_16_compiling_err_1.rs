use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, File, Ident, Item, ItemUnion, Lit, Meta, MetaNameValue, NestedMeta, parse_quote,
    punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Union_Alignment_16;

impl Mutator for Modify_Union_Alignment_16 {
    fn name(&self) -> &str {
        "Modify_Union_Alignment_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut new_attrs = Vec::new();
                for attr in &item_union.attrs {
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        if meta_list.path.is_ident("repr") {
                            let mut modified = false;
                            let mut new_meta_list = meta_list.clone();
                            for nested_meta in &mut new_meta_list.nested {
                                if let NestedMeta::Meta(Meta::NameValue(MetaNameValue { path, lit: Lit::Int(lit_int), .. })) = nested_meta {
                                    if path.is_ident("packed") {
                                        let new_value = match lit_int.base10_parse::<u64>() {
                                            Ok(1) => 2,
                                            Ok(2) => 4,
                                            Ok(4) => 8,
                                            _ => 1,
                                        };
                                        *nested_meta = NestedMeta::Meta(Meta::NameValue(MetaNameValue {
                                            path: path.clone(),
                                            eq_token: token::Eq { spans: [Span::call_site()] },
                                            lit: Lit::Int(LitInt::new(&new_value.to_string(), Span::call_site())),
                                        }));
                                        modified = true;
                                    }
                                }
                            }
                            if modified {
                                new_attrs.push(Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    path: meta_list.path.clone(),
                                    tokens: quote! { (#new_meta_list) },
                                });
                            } else {
                                new_attrs.push(attr.clone());
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                item_union.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}