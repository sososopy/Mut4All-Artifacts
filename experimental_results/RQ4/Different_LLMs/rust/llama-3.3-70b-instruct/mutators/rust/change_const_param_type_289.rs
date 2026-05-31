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

pub struct Change_Const_Param_Type_289;

impl Mutator for Change_Const_Param_Type_289 {
    fn name(&self) -> &str {
        "Change_Const_Param_Type_289"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_type = match const_param.ty {
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: ref segments,
                                },
                            }) => {
                                let segment = segments.iter().next().unwrap();
                                match segment.ident.to_string().as_str() {
                                    "i32" => parse_quote!(bool),
                                    "bool" => parse_quote!(char),
                                    "char" => parse_quote!(i32),
                                    _ => parse_quote!(i32),
                                }
                            }
                            _ => parse_quote!(i32),
                        };
                        const_param.ty = new_type;
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                let new_type = match const_param.ty {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: ref segments,
                                        },
                                    }) => {
                                        let segment = segments.iter().next().unwrap();
                                        match segment.ident.to_string().as_str() {
                                            "i32" => parse_quote!(bool),
                                            "bool" => parse_quote!(char),
                                            "char" => parse_quote!(i32),
                                            _ => parse_quote!(i32),
                                        }
                                    }
                                    _ => parse_quote!(i32),
                                };
                                const_param.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the type of const parameters in generic functions and impls. It aims to test the compiler's handling of different const parameter types, potentially exposing bugs related to type checking, inference, and const evaluation."
    }
}