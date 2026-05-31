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
                    if let Meta::List(meta_list) = attr.parse_meta().unwrap() {
                        if meta_list.path.is_ident("repr") {
                            let mut modified = false;
                            let mut new_meta_list = meta_list.clone();
                            let mut nested_meta_iter = new_meta_list.tokens.clone().into_iter();
                            let mut new_tokens = TokenStream::new();
                            while let Some(token) = nested_meta_iter.next() {
                                if let proc_macro2::TokenTree::Ident(ident) = &token {
                                    if ident == "packed" {
                                        if let Some(proc_macro2::TokenTree::Punct(punct)) = nested_meta_iter.next() {
                                            if punct.as_char() == '=' {
                                                if let Some(proc_macro2::TokenTree::Literal(literal)) = nested_meta_iter.next() {
                                                    let lit_str = literal.to_string();
                                                    if let Ok(lit_int) = lit_str.parse::<u64>() {
                                                        let new_value = match lit_int {
                                                            1 => 2,
                                                            2 => 4,
                                                            4 => 8,
                                                            _ => 1,
                                                        };
                                                        new_tokens.extend(quote! { packed = #new_value });
                                                        modified = true;
                                                        continue;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                new_tokens.extend(quote! { #token });
                            }
                            if modified {
                                new_attrs.push(Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    meta: Meta::List(MetaList {
                                        path: meta_list.path.clone(),
                                        delimiter: meta_list.delimiter,
                                        tokens: new_tokens,
                                    }),
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