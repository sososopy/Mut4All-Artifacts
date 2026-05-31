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

pub struct Modify_Enum_Representation_For_Alignment_461;

impl Mutator for Modify_Enum_Representation_For_Alignment_461 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_For_Alignment_461"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_repr_c = false;
                let mut new_attrs = Vec::new();
                for attr in &item_enum.attrs {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(meta_list)) = attr.meta() {
                            let mut new_nested = Punctuated::new();
                            let mut has_packed = false;
                            for nested_meta in meta_list
                                .parse_args_with(Punctuated::<Meta, token::Comma>::parse_terminated)
                                .unwrap()
                                .iter()
                            {
                                if let Meta::Path(path) = nested_meta {
                                    if path.is_ident("C") {
                                        has_repr_c = true;
                                        new_nested.push(nested_meta.clone());
                                    } else {
                                        new_nested.push(nested_meta.clone());
                                    }
                                } else if let Meta::List(list) = nested_meta {
                                    if list.path.is_ident("packed") {
                                        has_packed = true;
                                    } else {
                                        new_nested.push(nested_meta.clone());
                                    }
                                } else {
                                    new_nested.push(nested_meta.clone());
                                }
                            }
                            if has_packed {
                                let mut align_values = vec![1, 2, 4, 8, 16];
                                align_values.shuffle(&mut thread_rng());
                                if let Some(new_align) = align_values
                                    .into_iter()
                                    .find(|&align| !new_nested.iter().any(|nested| match nested {
                                        Meta::List(list) => {
                                            list.path.is_ident("align")
                                                && list
                                                    .parse_args::<syn::LitInt>()
                                                    .map_or(false, |lit| {
                                                        lit.base10_parse::<u64>()
                                                            .map_or(false, |value| value == align)
                                                    })
                                        }
                                        _ => false,
                                    }))
                                {
                                    new_nested.push(parse_quote!(align(#new_align)));
                                }
                            }
                            let new_meta_list = Meta::List(MetaList {
                                path: meta_list.path.clone(),
                                delimiter: meta_list.delimiter,
                                tokens: new_nested.to_token_stream(),
                            });
                            new_attrs.push(syn::Attribute {
                                pound_token: attr.pound_token,
                                style: attr.style,
                                bracket_token: attr.bracket_token,
                                meta: new_meta_list,
                            });
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                if has_repr_c {
                    item_enum.attrs = new_attrs;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with `#[repr(C, packed(N))]` attributes. It removes the `packed(N)` attribute and replaces it with `#[repr(C, align(M))]`, where `M` is a different alignment value from a shuffled list of common alignments. This transformation aims to test the compiler's handling of enum alignment and ABI compatibility by altering the representation attributes."
    }
}