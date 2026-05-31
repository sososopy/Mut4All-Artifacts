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

pub struct Mismatch_Attr_Delimiters_230;

impl Mutator for Mismatch_Attr_Delimiters_230 {
    fn name(&self) -> &str {
        "Mismatch_Attr_Delimiters_230"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                for attr in &mut struct_item.attrs {
                    if let Ok(meta) = attr.parse_meta() {
                        if let syn::Meta::List(list) = meta {
                            let path = &list.path;
                            let nested = &list.nested;
                            let new_tokens = quote! {
                                #[#path { #(#nested),* }]
                            };
                            let new_attrs = syn::Attribute::parse_outer(new_tokens).unwrap();
                            if let Some(new_attr) = new_attrs.into_iter().next() {
                                *attr = new_attr;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    for attr in &mut variant.attrs {
                        if let Ok(meta) = attr.parse_meta() {
                            if let syn::Meta::List(list) = meta {
                                let path = &list.path;
                                let nested = &list.nested;
                                let new_tokens = quote! {
                                    #[#path { #(#nested),* }]
                                };
                                let new_attrs = syn::Attribute::parse_outer(new_tokens).unwrap();
                                if let Some(new_attr) = new_attrs.into_iter().next() {
                                    *attr = new_attr;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro-generated struct and enum attributes by replacing closing delimiters with mismatched types (e.g., changing ')' to '}'). This forces the parser to handle invalid attribute syntax, exposing bugs in delimiter tracking during macro expansion and attribute parsing."
    }
}