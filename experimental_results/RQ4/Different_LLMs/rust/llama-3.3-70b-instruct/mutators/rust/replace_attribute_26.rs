use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Attribute_26;

impl Mutator for Replace_Attribute_26 {
    fn name(&self) -> &str {
        "Replace_Attribute_26"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let attributes: Vec<Attribute> = vec![
            parse_quote!(#[cfg(target_pointer_width = "64")]),
            parse_quote!(#[cfg(not(any(all(target_arch = "x86_64"))))]),
            parse_quote!(#[cfg(target_os = "windows")]),
            parse_quote!(#[cfg(not(target_os = "macos"))]),
        ];

        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    for attr in &mut func.attrs {
                        if rng.gen_bool(0.5) {
                            *attr = attributes.choose(&mut rng).unwrap().clone();
                        }
                    }
                }
                Item::Struct(struct_) => {
                    for attr in &mut struct_.attrs {
                        if rng.gen_bool(0.5) {
                            *attr = attributes.choose(&mut rng).unwrap().clone();
                        }
                    }
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing attributes with different ones. This transformation tests the compiler's handling of various attributes and their interactions with the code, potentially leading to errors or unexpected behavior."
    }
}