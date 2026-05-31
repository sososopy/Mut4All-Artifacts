use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, File, Ident, Item, ItemUnion, Lit, Meta, MetaNameValue, parse_quote,
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
                    let mut modified = false;
                    let mut new_tokens = TokenStream::new();
                    attr.parse_nested_meta(|meta| {
                        if let Meta::NameValue(MetaNameValue { path, value: Lit::Int(lit_int), .. }) = meta {
                            if path.is_ident("packed") {
                                let new_value = match lit_int.base10_parse::<u64>().unwrap() {
                                    1 => 2,
                                    2 => 4,
                                    4 => 8,
                                    _ => 1,
                                };
                                new_tokens.extend(quote! { packed = #new_value });
                                modified = true;
                            }
                        }
                        Ok(())
                    }).ok();

                    if modified {
                        new_attrs.push(Attribute {
                            pound_token: attr.pound_token,
                            style: attr.style,
                            bracket_token: attr.bracket_token,
                            meta: syn::Meta::List(syn::MetaList {
                                path: attr.path().clone(),
                                delimiter: syn::MacroDelimiter::Paren(Default::default()),
                                tokens: new_tokens,
                            }),
                        });
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