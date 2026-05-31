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

pub struct Replace_Type_Alias_With_Underlying_Type_286;

impl Mutator for Replace_Type_Alias_With_Underlying_Type_286 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Underlying_Type_286"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = HashMap::new();

        // Identify all type alias definitions in the code
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        segments,
                        ..
                    },
                    ..
                }) = *item_type.ty
                {
                    if let Some(segment) = segments.last() {
                        type_aliases.insert(segment.ident.to_string(), item_type.ty.clone());
                    }
                }
            }
        }

        // Replace all occurrences of the alias with its underlying type
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                segments,
                                ..
                            },
                            ..
                        }) = *pat_type.ty
                        {
                            if let Some(segment) = segments.last() {
                                if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                    pat_type.ty = underlying_type.clone();
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for arg in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        segments,
                                        ..
                                    },
                                    ..
                                }) = *pat_type.ty
                                {
                                    if let Some(segment) = segments.last() {
                                        if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                            pat_type.ty = underlying_type.clone();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            segments,
                            ..
                        },
                        ..
                    }) = field.ty
                    {
                        if let Some(segment) = segments.last() {
                            if let Some(underlying_type) = type_aliases.get(&segment.ident.to_string()) {
                                field.ty = underlying_type.clone();
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with their underlying types. It aims to test the compiler's handling of type aliases and their underlying types, potentially revealing issues with the type system, const evaluation, and other related features."
    }
}