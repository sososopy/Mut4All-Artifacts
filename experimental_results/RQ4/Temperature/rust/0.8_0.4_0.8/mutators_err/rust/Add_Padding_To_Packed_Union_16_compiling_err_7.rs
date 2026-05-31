use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, Meta, MetaNameValue, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Add_Padding_To_Packed_Union_16;

impl Mutator for Add_Padding_To_Packed_Union_16 {
    fn name(&self) -> &str {
        "Add_Padding_To_Packed_Union_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(item_union) = item {
                if let Some(attr) = item_union.attrs.iter().find(|attr| {
                    attr.path().is_ident("repr") && attr.parse_args_with(|input: syn::parse::ParseStream| {
                        input.parse::<Meta>().ok().and_then(|meta| {
                            if let Meta::List(meta_list) = meta {
                                meta_list.parse_nested_meta(|nested_meta| {
                                    if nested_meta.path.is_ident("packed") {
                                        return Ok(());
                                    }
                                    Err(syn::Error::new_spanned(nested_meta.path, "expected 'packed'"))
                                }).is_ok()
                            } else {
                                false
                            }
                        }).unwrap_or(false)
                    }).is_ok()
                }) {
                    let packed_value = attr.parse_args_with(|input: syn::parse::ParseStream| {
                        input.parse::<syn::LitInt>().and_then(|lit_int| lit_int.base10_parse::<usize>())
                    });

                    if let Ok(n) = packed_value {
                        let largest_field_size = item_union.fields.named.iter().map(|field| {
                            match &field.ty {
                                Type::Path(type_path) => {
                                    match type_path.path.segments.last().unwrap().ident.to_string().as_str() {
                                        "u8" => 1,
                                        "u16" => 2,
                                        "u32" => 4,
                                        "u64" => 8,
                                        _ => 0,
                                    }
                                }
                                _ => 0,
                            }
                        }).max().unwrap_or(0);

                        if largest_field_size < n {
                            let padding_size = n - largest_field_size;
                            let padding_field: syn::Field = parse_quote! {
                                _padding: [u8; #padding_size]
                            };
                            item_union.fields.named.push(padding_field);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unions with #[repr(packed(N))] and calculates the necessary padding to align the union's layout with the specified packed alignment. It introduces a padding field if the largest field size is less than the packed alignment size, ensuring correct layout alignment."
    }
}