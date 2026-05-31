use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, NestedMeta, Pat, PatType, Path as SynPath,
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

pub struct Modify_Union_Packing_Attributes_16;

impl Mutator for Modify_Union_Packing_Attributes_16 {
    fn name(&self) -> &str {
        "Modify_Union_Packing_Attributes_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(item_union) = item {
                if let Some(attr) = item_union.attrs.iter_mut().find(|attr| {
                    attr.meta.path().is_ident("repr") && attr.meta.to_token_stream().to_string().contains("packed")
                }) {
                    if let Meta::List(meta_list) = &attr.meta {
                        for nested_meta in &meta_list.nested {
                            if let NestedMeta::Meta(Meta::NameValue(meta_name_value)) = nested_meta {
                                if meta_name_value.path.is_ident("packed") {
                                    if let syn::Lit::Int(lit_int) = &meta_name_value.value {
                                        let current_value: u64 = lit_int.base10_parse().unwrap_or(1);
                                        let new_value = if current_value == 1 { 2 } else { 1 };
                                        *meta_name_value.value = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span()));
                                    }
                                }
                            }
                        }
                    }
                    attr.meta = Meta::List(meta_list.clone());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets union definitions with #[repr(packed(N))] attributes. It changes the packing alignment value to a different valid integer, such as switching from 1 to 2 or vice versa. This transformation is intended to explore the Rust compiler's behavior with different alignment constraints, potentially uncovering alignment-related issues."
    }
}