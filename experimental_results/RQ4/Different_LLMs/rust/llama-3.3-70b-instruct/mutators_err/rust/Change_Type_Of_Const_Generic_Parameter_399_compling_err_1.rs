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

pub struct Change_Type_Of_Const_Generic_Parameter_399;

impl Mutator for Change_Type_Of_Const_Generic_Parameter_399 {
    fn name(&self) -> &str {
        "Change_Type_Of_Const_Generic_Parameter_399"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_) = item {
                if let Some(generics) = &mut struct_.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_type = match &*param.ty {
                                Type::Path(TypePath {
                                    path: SynPath {
                                        segments: segments,
                                        ..
                                    },
                                    ..
                                }) if segments.len() == 1 => {
                                    let segment = &segments[0];
                                    match segment.ident.to_string().as_str() {
                                        "i32" => parse_quote!(u32),
                                        "u32" => parse_quote!(i64),
                                        "i64" => parse_quote!(usize),
                                        _ => continue,
                                    }
                                }
                                _ => continue,
                            };
                            param.ty = new_type;
                        }
                    }
                }
            } else if let Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_type = match &*param.ty {
                                Type::Path(TypePath {
                                    path: SynPath {
                                        segments: segments,
                                        ..
                                    },
                                    ..
                                }) if segments.len() == 1 => {
                                    let segment = &segments[0];
                                    match segment.ident.to_string().as_str() {
                                        "i32" => parse_quote!(u32),
                                        "u32" => parse_quote!(i64),
                                        "i64" => parse_quote!(usize),
                                        _ => continue,
                                    }
                                }
                                _ => continue,
                            };
                            param.ty = new_type;
                        }
                    }
                }
            } else if let Item::Impl(impl_) = item {
                if let Some(generics) = &mut impl_.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_type = match &*param.ty {
                                Type::Path(TypePath {
                                    path: SynPath {
                                        segments: segments,
                                        ..
                                    },
                                    ..
                                }) if segments.len() == 1 => {
                                    let segment = &segments[0];
                                    match segment.ident.to_string().as_str() {
                                        "i32" => parse_quote!(u32),
                                        "u32" => parse_quote!(i64),
                                        "i64" => parse_quote!(usize),
                                        _ => continue,
                                    }
                                }
                                _ => continue,
                            };
                            param.ty = new_type;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the type of const generic parameters in structs, functions, and impls. It replaces the original type with a new type that is still within the bounds of the generic constraint, potentially exposing bugs related to type checking or code generation."
    }
}