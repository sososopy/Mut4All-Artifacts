use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Replace_Constant_Enum_Value_With_Alternative_Value_67;

impl Mutator for Replace_Constant_Enum_Value_With_Alternative_Value_67 {
    fn name(&self) -> &str {
        "Replace_Constant_Enum_Value_With_Alternative_Value_67"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Path(expr_path) = &*const_item.expr {
                    if let Some(enum_path) = get_enum_path(expr_path) {
                        if let Some(alternative_variant) = get_alternative_variant(enum_path, file) {
                            const_item.expr = parse_quote!(#enum_path::#alternative_variant);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant enum values with alternative variants from the same enum. This transformation tests the compiler's handling of constant evaluation and enum values, potentially exposing bugs related to ICEs or unexpected behavior."
    }
}

fn get_enum_path(expr_path: &ExprPath) -> Option<SynPath> {
    if expr_path.path.segments.len() == 1 {
        let segment = &expr_path.path.segments[0];
        if segment.arguments.is_empty() {
            return Some(expr_path.path.clone());
        }
    }
    None
}

fn get_alternative_variant(enum_path: SynPath, file: &syn::File) -> Option<Ident> {
    let mut variants = Vec::new();
    for item in &file.items {
        if let Item::Enum(enum_item) = item {
            if enum_item.ident == enum_path.segments[0].ident {
                for variant in &enum_item.variants {
                    variants.push(variant.ident.clone());
                }
            }
        }
    }
    if variants.is_empty() {
        return None;
    }
    let mut rng = thread_rng();
    let alternative_variant = variants.choose(&mut rng).unwrap();
    Some(alternative_variant.clone())
}