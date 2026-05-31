use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Generic_Const_Exprs_Feature_434;

impl Mutator for Replace_Generic_Const_Exprs_Feature_434 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Exprs_Feature_434"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.attrs {
            if let syn::AttrStyle::Inner(item) = item {
                if let syn::Meta::List(list) = &item.meta {
                    if let Some(ident) = &list.path.get_ident() {
                        if ident.to_string() == "feature" {
                            for meta in &mut list.nested {
                                if let syn::NestedMeta::Meta(meta) = meta {
                                    if let syn::Meta::Path(path) = meta {
                                        if let Some(ident) = path.get_ident() {
                                            if ident.to_string() == "generic_const_exprs" {
                                                *meta = syn::Meta::Path(syn::Path::from(Ident::new(
                                                    "non_lifetime_binders",
                                                    Span::call_site(),
                                                )));
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the use of generic const expressions in Rust code, specifically replacing the feature attribute that enables this functionality. It replaces the `generic_const_exprs` feature attribute with `non_lifetime_binders`, which is also related to generics but has different semantics. This transformation aims to test the robustness of the Rust compiler in handling different generic features and their interactions."
    }
}