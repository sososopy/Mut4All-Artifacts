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
    Attribute, Meta, MetaList, NestedMeta, Lit,
};

use crate::mutator::Mutator;

pub struct Modify_Enum_Alignment_461;

impl Mutator for Modify_Enum_Alignment_461 {
    fn name(&self) -> &str {
        "Modify_Enum_Alignment_461"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                let mut new_attrs = Vec::new();
                for attr in &enum_item.attrs {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            let mut has_c = false;
                            let mut has_packed = false;
                            let mut packed_value = None;
                            for nested in meta_list.nested.iter() {
                                if let NestedMeta::Meta(Meta::Path(path)) = nested {
                                    if path.is_ident("C") {
                                        has_c = true;
                                    }
                                }
                                if let NestedMeta::Meta(Meta::List(list)) = nested {
                                    if list.path.is_ident("packed") {
                                        has_packed = true;
                                        if let Some(NestedMeta::Lit(Lit::Int(lit_int))) = list.nested.first() {
                                            packed_value = Some(lit_int.base10_parse::<u32>().unwrap());
                                        }
                                    }
                                }
                            }
                            if has_c && has_packed {
                                let mut rng = thread_rng();
                                let new_alignment = if rng.gen_bool(0.5) {
                                    None
                                } else {
                                    Some((rng.gen_range(1..=8) as u32).next_power_of_two())
                                };
                                let new_attr = match new_alignment {
                                    None => parse_quote!(#[repr(C)]),
                                    Some(align) => parse_quote!(#[repr(C, align(#align))]),
                                };
                                new_attrs.push(new_attr);
                                continue;
                            }
                        }
                    }
                    new_attrs.push(attr.clone());
                }
                enum_item.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies enums with `#[repr(C, packed(N))]` attributes, changing or removing the `packed(N)` specification. It tests how different alignment strategies affect ABI and layout, potentially uncovering compiler bugs related to alignment handling."
    }
}