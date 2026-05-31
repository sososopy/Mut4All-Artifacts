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

pub struct Replace_Iterator_Return_Type_357;

impl Mutator for Replace_Iterator_Return_Type_357 {
    fn name(&self) -> &str {
        "Replace_Iterator_Return_Type_357"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut iterator_types = Vec::new();

        // Collect all iterator types in the file
        for item in &file.items {
            if let Item::Use(use_item) = item {
                if let syn::UseTree::Path(path) = &use_item.tree {
                    for segment in &path.path.segments {
                        if segment.ident == "IntoIter" {
                            iterator_types.push(segment.ident.clone());
                        }
                    }
                }
            }
        }

        // Replace return type of iterator functions
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        if !iterator_types.is_empty() {
                            let new_return_type = Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: iterator_types
                                            .choose(&mut thread_rng())
                                            .unwrap()
                                            .clone(),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            });

                            func.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return type of iterator functions with another iterator type. This transformation aims to test the compiler's handling of different iterator types and their interactions with generic const expressions."
    }
}