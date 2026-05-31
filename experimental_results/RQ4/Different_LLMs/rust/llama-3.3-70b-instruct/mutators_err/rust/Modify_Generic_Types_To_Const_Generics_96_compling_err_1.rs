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

pub struct Modify_Generic_Types_To_Const_Generics_96;

impl Mutator for Modify_Generic_Types_To_Const_Generics_96 {
    fn name(&self) -> &str {
        "Modify_Generic_Types_To_Const_Generics_96"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "Vec" {
                                    pat_type.ty = Box::new(Type::Array(
                                        syn::TypeArray {
                                            bracket_token: token::Bracket {
                                                span: Span::call_site(),
                                            },
                                            elem: Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![
                                                        PathSegment {
                                                            ident: Ident::new("u8", Span::call_site()),
                                                            arguments: PathArguments::None,
                                                        },
                                                    ]),
                                                },
                                            })),
                                            len: Box::new(Expr::Lit(ExprLit {
                                                attrs: Vec::new(),
                                                lit: Lit::Int(IntLit {
                                                    digits: "10".to_string(),
                                                    span: Span::call_site(),
                                                }),
                                            })),
                                        },
                                    ));
                                }
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(segment) = type_path.path.segments.last() {
                            if segment.ident == "Vec" {
                                *return_type = Box::new(Type::Array(
                                    syn::TypeArray {
                                        bracket_token: token::Bracket {
                                            span: Span::call_site(),
                                        },
                                        elem: Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![
                                                    PathSegment {
                                                        ident: Ident::new("u8", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    },
                                                ]),
                                            },
                                        })),
                                        len: Box::new(Expr::Lit(ExprLit {
                                            attrs: Vec::new(),
                                            lit: Lit::Int(IntLit {
                                                digits: "10".to_string(),
                                                span: Span::call_site(),
                                            }),
                                        })),
                                    },
                                ));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        for param in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = param {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        if segment.ident == "Vec" {
                                            pat_type.ty = Box::new(Type::Array(
                                                syn::TypeArray {
                                                    bracket_token: token::Bracket {
                                                        span: Span::call_site(),
                                                    },
                                                    elem: Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: syn::Path {
                                                            leading_colon: None,
                                                            segments: Punctuated::from_iter(vec![
                                                                PathSegment {
                                                                    ident: Ident::new("u8", Span::call_site()),
                                                                    arguments: PathArguments::None,
                                                                },
                                                            ]),
                                                        },
                                                    })),
                                                    len: Box::new(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: Lit::Int(IntLit {
                                                            digits: "10".to_string(),
                                                            span: Span::call_site(),
                                                        }),
                                                    })),
                                                },
                                            ));
                                        }
                                    }
                                }
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "Vec" {
                                        *return_type = Box::new(Type::Array(
                                            syn::TypeArray {
                                                bracket_token: token::Bracket {
                                                    span: Span::call_site(),
                                                },
                                                elem: Box::new(Type::Path(TypePath {
                                                    qself: None,
                                                    path: syn::Path {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![
                                                            PathSegment {
                                                                ident: Ident::new("u8", Span::call_site()),
                                                                arguments: PathArguments::None,
                                                            },
                                                        ]),
                                                    },
                                                })),
                                                len: Box::new(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: Lit::Int(IntLit {
                                                        digits: "10".to_string(),
                                                        span: Span::call_site(),
                                                    }),
                                                })),
                                            },
                                        ));
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
        "The mutation operator replaces generic types in function parameters and return types with const generic expressions, aiming to trigger potential issues with generic const expressions."
    }
}