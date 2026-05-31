use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Alignment_Of_Packed_Unions_16;

impl Mutator for Modify_Alignment_Of_Packed_Unions_16 {
    fn name(&self) -> &str {
        "Modify_Alignment_Of_Packed_Unions_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                let mut is_packed = false;
                let mut new_attrs = Vec::new();
                
                for attr in &union.attrs {
                    if attr.path.is_ident("repr") {
                        if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                            let mut new_meta_list = meta_list.clone();
                            for nested_meta in &mut new_meta_list.nested {
                                if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested_meta {
                                    if path.is_ident("packed") {
                                        is_packed = true;
                                    }
                                }
                                if let syn::NestedMeta::Meta(syn::Meta::NameValue(name_value)) = nested_meta {
                                    if name_value.path.is_ident("packed") {
                                        if let syn::Lit::Int(lit_int) = &name_value.lit {
                                            let old_alignment = lit_int.base10_parse::<u64>().unwrap_or(1);
                                            let new_alignment = if old_alignment == 1 { 2 } else { old_alignment + 1 };
                                            *nested_meta = syn::NestedMeta::Meta(syn::Meta::NameValue(syn::MetaNameValue {
                                                path: name_value.path.clone(),
                                                eq_token: name_value.eq_token,
                                                lit: syn::Lit::Int(syn::LitInt::new(&new_alignment.to_string(), lit_int.span())),
                                            }));
                                        }
                                    }
                                }
                            }
                            new_attrs.push(syn::Attribute {
                                pound_token: attr.pound_token,
                                style: attr.style,
                                bracket_token: attr.bracket_token,
                                path: attr.path.clone(),
                                tokens: quote! { (#new_meta_list) },
                            });
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }

                if is_packed {
                    union.attrs = new_attrs;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unions with the `#[repr(packed)]` attribute, modifying their alignment values. It incrementally increases the alignment to test for ABI and layout alignment mismatches, aiming to expose potential bugs in the handling of packed unions during compilation."
    }
}