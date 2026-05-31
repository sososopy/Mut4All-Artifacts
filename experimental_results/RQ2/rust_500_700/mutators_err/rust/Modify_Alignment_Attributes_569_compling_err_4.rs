use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, Pat, PatType, Path as SynPath,
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

pub struct Modify_Alignment_Attributes_569;

impl Mutator for Modify_Alignment_Attributes_569 {
    fn name(&self) -> &str {
        "Modify_Alignment_Attributes_569"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(attr) = item_struct.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                    attr.parse_nested_meta(|nested_meta| {
                        if let Meta::NameValue(name_value) = nested_meta.meta {
                            if name_value.path.is_ident("align") {
                                if let syn::Expr::Lit(syn::ExprLit { lit: syn::Lit::Int(lit_int), .. }) = &name_value.value {
                                    if let Ok(original_align) = lit_int.base10_parse::<u32>() {
                                        let new_align = match original_align {
                                            1 => 2,
                                            2 => 4,
                                            4 => 8,
                                            8 => 16,
                                            16 => 32,
                                            32 => 64,
                                            64 => 128,
                                            _ => 16,
                                        };
                                        name_value.value = syn::Expr::Lit(syn::ExprLit {
                                            lit: syn::Lit::Int(syn::LitInt::new(&new_align.to_string(), lit_int.span())),
                                            attrs: Vec::new(),
                                        });
                                    }
                                }
                            }
                        }
                        Ok(())
                    }).unwrap_or(());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}