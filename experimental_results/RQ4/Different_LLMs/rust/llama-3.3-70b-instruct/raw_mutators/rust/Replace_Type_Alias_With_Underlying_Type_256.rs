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

pub struct Replace_Type_Alias_With_Underlying_Type_256;

impl Mutator for Replace_Type_Alias_With_Underlying_Type_256 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Underlying_Type_256"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments,
                    },
                }) = &*item_type.ty
                {
                    let ident = segments.first().unwrap().ident.clone();
                    for other_item in &mut file.items {
                        if let syn::Item::Fn(func) = other_item {
                            for arg in &mut func.sig.inputs {
                                if let FnArg::Typed(pat_type) = arg {
                                    if let Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: arg_segments,
                                        },
                                    }) = &*pat_type.ty
                                    {
                                        if arg_segments.first().unwrap().ident == ident {
                                            *pat_type.ty = item_type.ty.clone();
                                        }
                                    }
                                }
                            }
                        }
                        if let syn::Item::Impl(item_impl) = other_item {
                            for impl_item in &mut item_impl.items {
                                if let syn::ImplItem::Fn(method) = impl_item {
                                    for arg in &mut method.sig.inputs {
                                        if let FnArg::Typed(pat_type) = arg {
                                            if let Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: arg_segments,
                                                },
                                            }) = &*pat_type.ty
                                            {
                                                if arg_segments.first().unwrap().ident == ident {
                                                    *pat_type.ty = item_type.ty.clone();
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with their underlying types in function and method signatures, aiming to test the compiler's ability to resolve types and handle diagnostics in the presence of type aliases."
    }
}