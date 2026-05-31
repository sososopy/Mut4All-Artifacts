use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Remove_Attribute_27;

impl Mutator for Remove_Attribute_27 {
    fn name(&self) -> &str {
        "Remove_Attribute_27"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                struct_item.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || attr.parse_args::<syn::MetaList>().unwrap().nested.iter().any(|meta| {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = meta {
                            path.is_ident("align")
                        } else {
                            false
                        }
                    })
                });
            } else if let Item::Enum(enum_item) = item {
                enum_item.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || attr.parse_args::<syn::MetaList>().unwrap().nested.iter().any(|meta| {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = meta {
                            path.is_ident("align")
                        } else {
                            false
                        }
                    })
                });
            } else if let Item::Union(union_item) = item {
                union_item.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || attr.parse_args::<syn::MetaList>().unwrap().nested.iter().any(|meta| {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = meta {
                            path.is_ident("align")
                        } else {
                            false
                        }
                    })
                });
            } else if let Item::Fn(func_item) = item {
                func_item.attrs.retain(|attr| {
                    !attr.path.is_ident("repr") || attr.parse_args::<syn::MetaList>().unwrap().nested.iter().any(|meta| {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = meta {
                            path.is_ident("align")
                        } else {
                            false
                        }
                    })
                });
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Remove_Attribute_27 mutation operator removes the #[repr(align)] attribute from structs, enums, unions, and functions. This transformation tests how the Rust compiler handles the absence of specific representation attributes, potentially uncovering bugs or inconsistencies in how these attributes are processed or verified by the compiler."
    }
}