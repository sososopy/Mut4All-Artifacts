use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Meta, MetaList, Pat, PatType, Path as SynPath,
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

pub struct Misconfigured_Repr_Alignment_671;

impl Mutator for Misconfigured_Repr_Alignment_671 {
    fn name(&self) -> &str {
        "Misconfigured_Repr_Alignment_671"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(attr) = item_struct.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                        let mut found_align = false;
                        attr.parse_nested_meta(|meta| {
                            if meta.path.is_ident("align") {
                                found_align = true;
                            }
                            Ok(())
                        }).ok();
                        if found_align {
                            *attr = parse_quote!(#[repr(align("invalid"))]);
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    if let Some(attr) = item_enum.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                        let mut found_align = false;
                        attr.parse_nested_meta(|meta| {
                            if meta.path.is_ident("align") {
                                found_align = true;
                            }
                            Ok(())
                        }).ok();
                        if found_align {
                            *attr = parse_quote!(#[repr(align)]);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}