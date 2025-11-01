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

pub struct Modify_Struct_Union_Alignments_326;

impl Mutator for Modify_Struct_Union_Alignments_326 {
    fn name(&self) -> &str {
        "Modify_Struct_Union_Alignments_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ref mut item_struct) = item {
                item_struct.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || !attr.tokens.to_string().contains("packed")
                });
            }
            if let Item::Union(ref mut item_union) = item {
                item_union.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || !attr.tokens.to_string().contains("packed")
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies structs and unions with explicit alignment attributes, specifically #[repr(packed)], and removes this attribute. The goal is to expose potential bugs related to data alignment and memory layout assumptions. By removing the packing attribute, the compiler may arrange data differently in memory, revealing inconsistencies and misalignments."
    }
}