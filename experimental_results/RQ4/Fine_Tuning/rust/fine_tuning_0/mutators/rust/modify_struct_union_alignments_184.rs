use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Struct_Union_Alignments_184;

impl Mutator for Modify_Struct_Union_Alignments_184 {
    fn name(&self) -> &str {
        "Modify_Struct_Union_Alignments_184"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(attr_index) = item_struct.attrs.iter().position(|attr| {
                        attr.path().is_ident("repr") && attr.meta.to_token_stream().to_string().contains("packed")
                    }) {
                        item_struct.attrs.remove(attr_index);
                    }
                }
                Item::Union(item_union) => {
                    if let Some(attr_index) = item_union.attrs.iter().position(|attr| {
                        attr.path().is_ident("repr") && attr.meta.to_token_stream().to_string().contains("packed")
                    }) {
                        item_union.attrs.remove(attr_index);
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs and unions with the `#[repr(packed)]` attribute and removes this attribute. This transformation can expose issues related to data alignment and memory layout assumptions by altering how the compiler arranges data in memory. It aims to uncover bugs related to ABI compliance and alignment expectations."
    }
}