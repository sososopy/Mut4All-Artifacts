use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Change_Alignment_179;

impl Mutator for Change_Alignment_179 {
    fn name(&self) -> &str {
        "Change_Alignment_179"
    }

    fn mutate(&self, file: &mut syn::File) {
        let alignments = vec![1, 2, 4, 8];
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for attr in &mut struct_item.attrs {
                    if let Attribute { meta,.. } = attr {
                        if let Meta::Path(path) = meta {
                            if path.is_ident("repr") {
                                if let Some(packed) = path.segments.last() {
                                    if packed.ident == "packed" {
                                        let new_alignment = alignments.choose(&mut thread_rng()).unwrap();
                                        *attr = parse_quote!(#[repr(packed(#new_alignment))]);
                                    }
                                }
                            }
                        } else if let Meta::List(list) = meta {
                            if list.path.is_ident("repr") {
                                for meta in list.nested {
                                    if let NestedMeta::Meta(Meta::NameValue(name_value)) = meta {
                                        if name_value.path.is_ident("packed") {
                                            let new_alignment = alignments.choose(&mut thread_rng()).unwrap();
                                            *attr = parse_quote!(#[repr(packed(#new_alignment))]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Union(union_item) = item {
                for attr in &mut union_item.attrs {
                    if let Attribute { meta,.. } = attr {
                        if let Meta::Path(path) = meta {
                            if path.is_ident("repr") {
                                if let Some(packed) = path.segments.last() {
                                    if packed.ident == "packed" {
                                        let new_alignment = alignments.choose(&mut thread_rng()).unwrap();
                                        *attr = parse_quote!(#[repr(packed(#new_alignment))]);
                                    }
                                }
                            }
                        } else if let Meta::List(list) = meta {
                            if list.path.is_ident("repr") {
                                for meta in list.nested {
                                    if let NestedMeta::Meta(Meta::NameValue(name_value)) = meta {
                                        if name_value.path.is_ident("packed") {
                                            let new_alignment = alignments.choose(&mut thread_rng()).unwrap();
                                            *attr = parse_quote!(#[repr(packed(#new_alignment))]);
                                        }
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
        "The mutation operator changes the alignment attribute of structs and unions. It randomly selects a new alignment value from a predefined set and replaces the existing alignment. This transformation tests the compiler's handling of different alignments and their impact on the layout of structs and unions."
    }
}

fn main() {
    let mut file_ast = parse_quote! {
        #[repr(packed)]
        struct Test {
            a: u8,
            b: u16,
        }
    };
    Change_Alignment_179.mutate(&mut file_ast);
}