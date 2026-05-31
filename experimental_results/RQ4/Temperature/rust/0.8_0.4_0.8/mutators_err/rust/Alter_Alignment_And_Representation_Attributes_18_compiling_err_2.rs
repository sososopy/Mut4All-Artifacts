use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Meta, Pat, PatType, Path as SynPath,
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

pub struct Alter_Alignment_And_Representation_Attributes_18;

impl Mutator for Alter_Alignment_And_Representation_Attributes_18 {
    fn name(&self) -> &str {
        "Alter_Alignment_And_Representation_Attributes_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(attr_index) = item_struct.attrs.iter().position(|attr| attr.path().is_ident("repr")) {
                        if let Meta::List(meta_list) = &item_struct.attrs[attr_index].meta {
                            if meta_list.nested.iter().any(|nested_meta| nested_meta.to_token_stream().to_string().contains("packed")) {
                                item_struct.attrs[attr_index] = parse_quote!(#[repr(C)]);
                            }
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    if let Some(attr_index) = item_enum.attrs.iter().position(|attr| attr.path().is_ident("repr")) {
                        if let Meta::List(meta_list) = &item_enum.attrs[attr_index].meta {
                            if meta_list.nested.iter().any(|nested_meta| nested_meta.to_token_stream().to_string().contains("packed")) {
                                item_enum.attrs[attr_index] = parse_quote!(#[repr(C)]);
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs and enums with the `#[repr(packed)]` attribute and changes it to `#[repr(C)]`. It aims to test the compiler's handling of different data layouts and alignments, potentially exposing bugs related to ABI and data layout inconsistencies."
    }
}