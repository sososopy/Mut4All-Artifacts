use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut, *,
    Attribute, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident, Item,
    ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, NestedMeta,
    Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound,
    TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath,
};

use crate::mutator::Mutator;

pub struct Modify_Union_Field_Alignment_16;

impl Mutator for Modify_Union_Field_Alignment_16 {
    fn name(&self) -> &str {
        "Modify_Union_Field_Alignment_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                if let Some(attr) = union.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        for nested_meta in meta_list.nested.iter_mut() {
                            if let NestedMeta::Meta(Meta::NameValue(name_value)) = nested_meta {
                                if name_value.path.is_ident("packed") {
                                    if let syn::Lit::Int(lit_int) = &name_value.value {
                                        if let Ok(value) = lit_int.base10_parse::<u8>() {
                                            let new_value = if value == 1 { 2 } else { 1 };
                                            name_value.value = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span()));
                                        }
                                    }
                                }
                            }
                        }
                        attr.meta = Meta::List(meta_list);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets unions with the #[repr(packed)] attribute, specifically modifying the packed alignment value. By changing the alignment from 1 to 2 (or vice versa), it aims to reveal potential misalignments and inconsistencies in the union's field layout as processed by the Rust compiler. This can expose latent bugs related to layout computation and alignment handling."
    }
}