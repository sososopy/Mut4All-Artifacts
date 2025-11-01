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

pub struct Replace_Unsized_Types_25;

impl Mutator for Replace_Unsized_Types_25 {
    fn name(&self) -> &str {
        "Replace_Unsized_Types_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { fields, .. }) = item {
                for field in fields.iter_mut() {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident == "str" {
                                field.ty = parse_quote!([u8; 32]);
                            } else if segment.ident == "slice" {
                                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                    if let Some(GenericArgument::Type(inner_ty)) = args.args.first() {
                                        field.ty = parse_quote!([#inner_ty; 10]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions that include unsized types, such as `str` or `[T]`. It replaces these with fixed-size arrays to ensure the struct is fully sized. This helps prevent compiler issues related to memory layout evaluations by assuming a fixed size for fields that were previously unsized."
    }
}