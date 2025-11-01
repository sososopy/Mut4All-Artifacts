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

pub struct Modify_Enum_Alignment_428;

impl Mutator for Modify_Enum_Alignment_428 {
    fn name(&self) -> &str {
        "Modify_Enum_Alignment_428"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut new_attrs = Vec::new();
                for attr in &item_enum.attrs {
                    if attr.path.is_ident("repr") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                let mut has_packed = false;
                                let mut new_meta_list = meta_list.clone();
                                new_meta_list.nested = Punctuated::new();
                                for nested_meta in meta_list.nested {
                                    if let NestedMeta::Meta(Meta::Path(path)) = &nested_meta {
                                        if path.is_ident("C") {
                                            new_meta_list.nested.push(nested_meta.clone());
                                        }
                                    } else if let NestedMeta::Meta(Meta::List(meta_list)) = &nested_meta {
                                        if meta_list.path.is_ident("packed") {
                                            has_packed = true;
                                        } else {
                                            new_meta_list.nested.push(nested_meta.clone());
                                        }
                                    }
                                }
                                if has_packed {
                                    if thread_rng().gen_bool(0.5) {
                                        new_attrs.push(parse_quote!(#[repr(C)]));
                                    } else {
                                        let align_value = [2, 4, 8].choose(&mut thread_rng()).unwrap();
                                        new_attrs.push(parse_quote!(#[repr(C, align(#align_value))]));
                                    }
                                } else {
                                    new_attrs.push(attr.clone());
                                }
                            }
                        }
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                item_enum.attrs = new_attrs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies enums with a `#[repr(C, packed(N))]` attribute and modifies the packing alignment or removes it. The mutation either changes the packing to a different alignment value or removes the packing attribute, replacing it with `#[repr(C)]` or `#[repr(C, align(M))]`. This approach tests different alignment strategies to identify potential ABI and layout mismatches."
    }
}