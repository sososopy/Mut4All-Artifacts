use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Const_Generic_Parameters_401;

impl Mutator for Modify_Const_Generic_Parameters_401 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.iter_mut().find(|param| {
                    if let GenericParam::Const(const_param) = param {
                        !matches!(
                            const_param.ty,
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    ref segments,
                                },
                            }) if segments.len() == 1 && matches!(
                                segments.first().unwrap().ident.to_string().as_str(),
                                "i8" | "i16" | "i32" | "i64" | "i128" | "isize"
                                    | "u8" | "u16" | "u32" | "u64" | "u128" | "usize"
                                    | "bool" | "char"
                            )
                        )
                    } else {
                        false
                    }
                }) {
                    if let GenericParam::Const(const_param) = generics {
                        const_param.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("i32", Span::call_site())),
                        }));
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                if let Some(generics) = item_fn.sig.generics.params.iter_mut().find(|param| {
                    if let GenericParam::Const(const_param) = param {
                        !matches!(
                            const_param.ty,
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    ref segments,
                                },
                            }) if segments.len() == 1 && matches!(
                                segments.first().unwrap().ident.to_string().as_str(),
                                "i8" | "i16" | "i32" | "i64" | "i128" | "isize"
                                    | "u8" | "u16" | "u32" | "u64" | "u128" | "usize"
                                    | "bool" | "char"
                            )
                        )
                    } else {
                        false
                    }
                }) {
                    if let GenericParam::Const(const_param) = generics {
                        const_param.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("i32", Span::call_site())),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs and functions using const generics with unsupported types, such as user-defined types. It replaces these const generic parameters with a supported type, specifically `i32`. By leveraging the current limitations of Rust's const generics, this transformation aims to stress-test the compiler's type resolution and const evaluation mechanisms, potentially exposing bugs or ICEs related to const generic handling."
    }
}