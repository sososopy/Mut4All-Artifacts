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

pub struct Modify_Union_Field_Alignment_16;

impl Mutator for Modify_Union_Field_Alignment_16 {
    fn name(&self) -> &str {
        "Modify_Union_Field_Alignment_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(union) = item {
                if let Some(attr) = union.attrs.iter_mut().find(|attr| attr.path.is_ident("repr")) {
                    if let Ok(mut meta) = attr.parse_meta() {
                        if let syn::Meta::List(meta_list) = &mut meta {
                            for nested_meta in &mut meta_list.nested {
                                if let syn::NestedMeta::Meta(syn::Meta::NameValue(name_value)) = nested_meta {
                                    if name_value.path.is_ident("packed") {
                                        if let syn::Lit::Int(lit_int) = &name_value.lit {
                                            if let Ok(value) = lit_int.base10_parse::<u8>() {
                                                let new_value = if value == 1 { 2 } else { 1 };
                                                name_value.lit = syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), lit_int.span()));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        *attr = syn::Attribute {
                            pound_token: attr.pound_token,
                            style: attr.style,
                            bracket_token: attr.bracket_token,
                            path: attr.path.clone(),
                            tokens: quote! { (#meta) },
                        };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets unions with the #[repr(packed)] attribute, specifically modifying the packed alignment value. By changing the alignment from 1 to 2 (or vice versa), it aims to reveal potential misalignments and inconsistencies in the union's field layout as processed by the Rust compiler. This can expose latent bugs related to layout computation and alignment handling."
    }
}