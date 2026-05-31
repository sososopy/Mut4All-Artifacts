use proc_macro2::{Span, Ident};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Enum_Padding_Manipulation_603;

impl Mutator for Enum_Padding_Manipulation_603 {
    fn name(&self) -> &str {
        "Enum_Padding_Manipulation_603"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut has_repr = false;
                for attr in &item_enum.attrs {
                    if attr.path().is_ident("repr") {
                        has_repr = true;
                        break;
                    }
                }
                if has_repr {
                    // Modify the repr attribute
                    item_enum.attrs.retain(|attr| !attr.path().is_ident("repr"));
                    item_enum.attrs.push(parse_quote!(#[repr(packed)]));

                    // Introduce additional fields to the enum variants
                    for variant in &mut item_enum.variants {
                        if variant.fields.is_empty() {
                            variant.fields = syn::Fields::Unnamed(syn::FieldsUnnamed {
                                paren_token: token::Paren::default(),
                                unnamed: Punctuated::from_iter(vec![
                                    syn::Field {
                                        attrs: Vec::new(),
                                        vis: syn::Visibility::Inherited,
                                        ident: None,
                                        colon_token: None,
                                        mutability: syn::FieldMutability::None,
                                        ty: syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("u16", Span::call_site())),
                                        }),
                                    },
                                    syn::Field {
                                        attrs: Vec::new(),
                                        vis: syn::Visibility::Inherited,
                                        ident: None,
                                        colon_token: None,
                                        mutability: syn::FieldMutability::None,
                                        ty: syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("u32", Span::call_site())),
                                        }),
                                    },
                                ]),
                            });
                        }
                    }

                    // Create a function to access these new fields
                    file.items.push(parse_quote! {
                        fn access_enum_fields() {
                            let val = MyEnum::Variant3(5, 10);
                            match val {
                                MyEnum::Variant3(a, b) => {
                                    assert_eq!(a as u8, 5);
                                    assert_eq!(b as u16, 10);
                                },
                                _ => {}
                            }
                        }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}