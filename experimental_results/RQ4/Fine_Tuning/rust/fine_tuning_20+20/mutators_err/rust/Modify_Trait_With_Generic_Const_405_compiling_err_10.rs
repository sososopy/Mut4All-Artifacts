use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    str::FromStr,
};
use syn::{
    parse_quote,
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

pub struct Modify_Trait_With_Generic_Const_405;

impl Mutator for Modify_Trait_With_Generic_Const_405 {
    fn name(&self) -> &str {
        "Modify_Trait_With_Generic_Const_405"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_generic_const_exprs_feature = false;
        for attr in &file.attrs {
            if attr.path().is_ident("feature") {
                if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                    for nested_meta in meta_list.nested.iter() {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested_meta {
                            if path.is_ident("generic_const_exprs") {
                                has_generic_const_exprs_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_generic_const_exprs_feature {
            let feature_attr: syn::Attribute =
                parse_quote!(#![feature(generic_const_exprs)]);
            file.attrs.push(feature_attr);
        }
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut has_generic_const = false;
                for generics in &trait_item.generics.params {
                    if let syn::GenericParam::Const(_) = generics {
                        has_generic_const = true;
                        break;
                    }
                }
                if has_generic_const {
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Type(assoc_type) = item {
                            if let Some(const_param) = assoc_type.generics.const_params().next() {
                                let new_type = if const_param.ty == parse_quote!(i16) {
                                    parse_quote!(u8)
                                } else {
                                    parse_quote!(i16)
                                };
                                assoc_type.generics = assoc_type
                                    .generics
                                    .clone()
                                    .into_iter()
                                    .map(|param| match param {
                                        syn::GenericParam::Const(mut const_param) => {
                                            const_param.ty = new_type.clone();
                                            syn::GenericParam::Const(const_param)
                                        }
                                        _ => param,
                                    })
                                    .collect();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with generic const parameters, specifically those that use the parameter in an associated type. It modifies the const parameter's type, such as changing from `i16` to `u8`, and ensures the `#![feature(generic_const_exprs)]` attribute is present. This transformation tests the compiler's handling of const generics and associated types, especially under type changes, and can expose bugs in const evaluation and trait resolution."
    }
}