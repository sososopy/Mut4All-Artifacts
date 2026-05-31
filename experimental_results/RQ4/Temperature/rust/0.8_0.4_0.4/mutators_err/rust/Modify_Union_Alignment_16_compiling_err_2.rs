use proc_macro2::{Span, *};
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

pub struct Modify_Union_Alignment_16;

impl Mutator for Modify_Union_Alignment_16 {
    fn name(&self) -> &str {
        "Modify_Union_Alignment_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union_item) = item {
                let mut has_packed = false;
                for attr in &mut union_item.attrs {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            for nested_meta in meta_list.tokens.clone().into_iter() {
                                if let NestedMeta::Meta(Meta::NameValue(nv)) = nested_meta {
                                    if nv.path.is_ident("packed") {
                                        if let syn::Lit::Int(lit_int) = nv.value {
                                            if let Ok(n) = lit_int.base10_parse::<u8>() {
                                                let new_n = match n {
                                                    1 => 2,
                                                    2 => 4,
                                                    _ => 1,
                                                };
                                                *attr = parse_quote!(#[repr(packed(#new_n))]);
                                                has_packed = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if !has_packed {
                    union_item.attrs.push(parse_quote!(#[repr(packed(2))]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}