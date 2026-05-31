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

pub struct Replace_Trait_Bounds_263;

impl Mutator for Replace_Trait_Bounds_263 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_263"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_bounds = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let TraitItem::Method(method) = item {
                        for input in &method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    for segment in &type_path.path.segments {
                                        if let Some(trait_bound) = get_trait_bound(segment) {
                                            trait_bounds.push(trait_bound);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Method(method) = item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &mut *pat_type.ty {
                                    for segment in &mut type_path.path.segments {
                                        if let Some(trait_bound) = get_trait_bound(segment) {
                                            if let Some(new_trait_bound) = get_new_trait_bound(&trait_bounds) {
                                                segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath {
                                                            leading_colon: None,
                                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                                ident: Ident::new(new_trait_bound, Span::call_site()),
                                                                arguments: PathArguments::None,
                                                            }]),
                                                        },
                                                    }))]),
                                                    gt_token: token::Gt::default(),
                                                });
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
        "The mutation operator replaces existing trait bounds with new trait bounds. It first collects all trait bounds from the trait definitions, then replaces each trait bound with a new one from the collected list. This transformation aims to test the compiler's ability to handle different trait bounds and their interactions."
    }
}

fn get_trait_bound(segment: &PathSegment) -> Option<String> {
    segment.ident.to_string().parse().ok()
}

fn get_new_trait_bound(trait_bounds: &[String]) -> Option<String> {
    if trait_bounds.is_empty() {
        None
    } else {
        let mut rng = thread_rng();
        Some(trait_bounds.choose(&mut rng).unwrap().clone())
    }
}