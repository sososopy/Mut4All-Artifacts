use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    AttrStyle, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, MetaNameValue, Path as SynPath,
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

pub struct Replace_Specialization_With_Min_Specialization_27;

impl Mutator for Replace_Specialization_With_Min_Specialization_27 {
    fn name(&self) -> &str {
        "Replace_Specialization_With_Min_Specialization_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for attr in &mut file.attrs {
            if let Some(ident) = attr.path().get_ident() {
                if ident == "feature" {
                    if let Meta::List(meta_list) = attr.parse_meta().unwrap_or(Meta::List(MetaList {
                        path: attr.path().clone(),
                        delimiter: syn::MacroDelimiter::Paren(Default::default()),
                        tokens: proc_macro2::TokenStream::new(),
                    })) {
                        if meta_list.tokens.to_string().contains("specialization") {
                            attr.tokens = quote! {(min_specialization)};
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the `#![feature(specialization)]` attribute and replaces it with `#![feature(min_specialization)]`. This transformation aims to test the stability of code using the `min_specialization` feature, which is considered more stable than full specialization. By doing so, it evaluates whether this change can prevent potential ICEs or other compiler errors associated with the less stable specialization feature."
    }
}