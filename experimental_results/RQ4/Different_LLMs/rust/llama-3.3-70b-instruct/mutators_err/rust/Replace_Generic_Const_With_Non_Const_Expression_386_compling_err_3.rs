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

pub struct Replace_Generic_Const_With_Non_Const_Expression_386;

impl Mutator for Replace_Generic_Const_With_Non_Const_Expression_386 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_With_Non_Const_Expression_386"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = pat_type.ty.as_ref() {
                            for segment in segments {
                                if let Some(const_param) = &segment.arguments {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = const_param {
                                        for arg in &angle_bracketed.args {
                                            if let GenericArgument::Const(const_arg) = arg {
                                                let new_param = parse_quote! { let #const_arg = 3; };
                                                func.block.stmts.insert(0, Stmt::Local(Local {
                                                    attrs: vec![],
                                                    let_token: Default::default(),
                                                    pat: Box::new(Pat::Ident(Ident::new(
                                                        &const_arg.to_token_stream().to_string(),
                                                        Span::call_site(),
                                                    ))),
                                                    init: Some(LocalInit {
                                                        attrs: vec![],
                                                        eq_token: Default::default(),
                                                        expr: parse_quote! { 3 },
                                                    }),
                                                    semi_token: Some(Default::default()),
                                                }));
                                                func.sig.inputs.retain(|p| {
                                                    if let FnArg::Typed(ty) = p {
                                                        if let Type::Path(TypePath {
                                                            qself: None,
                                                            path: SynPath {
                                                                leading_colon: None,
                                                                segments,
                                                            },
                                                        }) = ty.ty.as_ref() {
                                                            segments
                                                                .iter()
                                                                .any(|s| s.ident == const_arg)
                                                        } else {
                                                            true
                                                        }
                                                    } else {
                                                        true
                                                    }
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        for param in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = param {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = pat_type.ty.as_ref() {
                                    for segment in segments {
                                        if let Some(const_param) = &segment.arguments {
                                            if let PathArguments::AngleBracketed(angle_bracketed) = const_param {
                                                for arg in &angle_bracketed.args {
                                                    if let GenericArgument::Const(const_arg) = arg {
                                                        let new_param = parse_quote! { let #const_arg = 3; };
                                                        func.block.stmts.insert(0, Stmt::Local(Local {
                                                            attrs: vec![],
                                                            let_token: Default::default(),
                                                            pat: Box::new(Pat::Ident(Ident::new(
                                                                &const_arg.to_token_stream().to_string(),
                                                                Span::call_site(),
                                                            ))),
                                                            init: Some(LocalInit {
                                                                attrs: vec![],
                                                                eq_token: Default::default(),
                                                                expr: parse_quote! { 3 },
                                                            }),
                                                            semi_token: Some(Default::default()),
                                                        }));
                                                        func.sig.inputs.retain(|p| {
                                                            if let FnArg::Typed(ty) = p {
                                                                if let Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath {
                                                                        leading_colon: None,
                                                                        segments,
                                                                    },
                                                                }) = ty.ty.as_ref() {
                                                                    segments
                                                                        .iter()
                                                                        .any(|s| s.ident == const_arg)
                                                                } else {
                                                                    true
                                                                }
                                                            } else {
                                                                true
                                                            }
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic const parameters with non-const expressions. It aims to test the compiler's handling of generic const expressions and their replacements with non-const expressions, potentially leading to internal errors or bugs in the Rust compiler."
    }
}