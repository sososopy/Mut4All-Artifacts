use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    default::Default,
};
use syn::{
    self, AttrStyle, Attribute, FnArg, GenericParam, Ident, Item, ItemFn, ItemStruct, Lit, Meta,
    PathArguments, PathSegment, ReturnType, Stmt, Token, Type, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    visit::Visit,
    visit_mut::VisitMut,
    Path as SynPath,
};

use crate::mutator::Mutator;

pub struct Mutator_CfgAttr_PathMutation_25;

impl Mutator for Mutator_CfgAttr_PathMutation_25 {
    fn name(&self) -> &str {
        "Mutator_CfgAttr_PathMutation_25"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    for attr in &mut func.attrs {
                        let meta = &mut attr.meta;
                        if let Meta::List(list) = meta {
                            if list.path.is_ident("cfg_attr") {
                                let nested_result = syn::parse2::<Punctuated<syn::parse::NestedMeta, syn::Token![,]>>(list.tokens.clone());
                                if let Ok(nested) = nested_result {
                                    if nested.len() >= 2 {
                                        let second = &nested[1];
                                        if let syn::parse::NestedMeta::Meta(meta) = second {
                                            if let Meta::Path(path) = meta {
                                                let mut new_segments = Punctuated::new();
                                                new_segments.push(PathSegment {
                                                    ident: Ident::new("cfg_attr", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                });
                                                for seg in &path.segments {
                                                    new_segments.push(seg.clone());
                                                }
                                                let new_path = SynPath {
                                                    leading_colon: None,
                                                    segments: new_segments,
                                                };
                                                list.path = new_path;
                                                attr.meta = meta.clone();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Struct(struc) => {
                    for attr in &mut struc.attrs {
                        let meta = &mut attr.meta;
                        if let Meta::List(list) = meta {
                            if list.path.is_ident("cfg_attr") {
                                let nested_result = syn::parse2::<Punctuated<syn::parse::NestedMeta, syn::Token![,]>>(list.tokens.clone());
                                if let Ok(nested) = nested_result {
                                    if nested.len() >= 2 {
                                        let second = &nested[1];
                                        if let syn::parse::NestedMeta::Meta(meta) = second {
                                            if let Meta::Path(path) = meta {
                                                let mut new_segments = Punctuated::new();
                                                new_segments.push(PathSegment {
                                                    ident: Ident::new("cfg_attr", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                });
                                                for seg in &path.segments {
                                                    new_segments.push(seg.clone());
                                                }
                                                let new_path = SynPath {
                                                    leading_colon: None,
                                                    segments: new_segments,
                                                };
                                                list.path = new_path;
                                                attr.meta = meta.clone();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms valid `cfg_attr` macro invocations into incorrect path-based attributes by replacing `cfg_attr!(...)` with `cfg_attr::...`. This forces the compiler to resolve `cfg_attr` as a module or crate path rather than a macro, exposing gaps in attribute resolution logic. The transformation is applied to all existing `cfg_attr` usages in attributes, ensuring broad coverage of attribute processing code."
    }
}