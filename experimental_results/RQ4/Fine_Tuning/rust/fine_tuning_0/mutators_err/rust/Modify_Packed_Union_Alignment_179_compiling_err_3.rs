use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, Pat, PatType, Path as SynPath,
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

pub struct Modify_Packed_Union_Alignment_179;

impl Mutator for Modify_Packed_Union_Alignment_179 {
    fn name(&self) -> &str {
        "Modify_Packed_Union_Alignment_179"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                if let Some(attr) = union.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        if meta_list.tokens.to_string().contains("packed") {
                            let mut max_alignment = 1;
                            for field in &union.fields.named {
                                if let syn::Type::Path(type_path) = &field.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        match segment.ident.to_string().as_str() {
                                            "u8" | "i8" => max_alignment = max_alignment.max(1),
                                            "u16" | "i16" => max_alignment = max_alignment.max(2),
                                            "u32" | "i32" | "f32" => max_alignment = max_alignment.max(4),
                                            "u64" | "i64" | "f64" => max_alignment = max_alignment.max(8),
                                            _ => {}
                                        }
                                    }
                                }
                            }
                            let new_alignment = max_alignment + 1;
                            let new_meta_list = MetaList {
                                path: syn::Path::from(Ident::new("repr", Span::call_site())),
                                delimiter: syn::MacroDelimiter::Paren(Default::default()),
                                tokens: quote!(packed(#new_alignment)).into(),
                            };
                            *attr = parse_quote!(#[#new_meta_list]);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets union declarations with the `#[repr(packed(n))]` attribute, increasing the packing alignment value beyond the natural alignment of any field. It aims to create potential misalignment situations, challenging the compiler's alignment handling and potentially uncovering internal errors or unexpected behaviors."
    }
}