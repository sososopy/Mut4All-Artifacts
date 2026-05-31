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

pub struct Modify_Function_With_Const_Generics_85;

impl Mutator for Modify_Function_With_Const_Generics_85 {
    fn name(&self) -> &str {
        "Modify_Function_With_Const_Generics_85"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, generics, _)) = &item_impl.generics.split_for_impl() {
                    if let Some(syn::GenericParam::Const(const_param)) = generics.params.first() {
                        if const_param.ident == "STRATEGIES" {
                            for impl_item in &mut item_impl.items {
                                if let syn::ImplItem::Fn(func) = impl_item {
                                    if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                                        *ty = Box::new(syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(syn::PathSegment {
                                                        ident: Ident::new("Combination", Span::call_site()),
                                                        arguments: syn::PathArguments::AngleBracketed(
                                                            syn::AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: {
                                                                    let mut args = Punctuated::new();
                                                                    args.push(syn::GenericArgument::Const(syn::Expr::Binary(syn::ExprBinary {
                                                                        attrs: vec![],
                                                                        left: Box::new(syn::Expr::Path(syn::ExprPath {
                                                                            attrs: vec![],
                                                                            qself: None,
                                                                            path: syn::Path::from(Ident::new("STRATEGIES", Span::call_site())),
                                                                        })),
                                                                        op: syn::BinOp::Mul(token::Star { spans: [Span::call_site()] }),
                                                                        right: Box::new(syn::Expr::Lit(syn::ExprLit {
                                                                            attrs: vec![],
                                                                            lit: syn::Lit::Int(syn::LitInt::new("2", Span::call_site())),
                                                                        })),
                                                                    })));
                                                                    args.push(syn::GenericArgument::Const(syn::Expr::Binary(syn::ExprBinary {
                                                                        attrs: vec![],
                                                                        left: Box::new(syn::Expr::Lit(syn::ExprLit {
                                                                            attrs: vec![],
                                                                            lit: syn::Lit::Int(syn::LitInt::new("1", Span::call_site())),
                                                                        })),
                                                                        op: syn::BinOp::Add(token::Plus { spans: [Span::call_site()] }),
                                                                        right: Box::new(syn::Expr::Path(syn::ExprPath {
                                                                            attrs: vec![],
                                                                            qself: None,
                                                                            path: syn::Path::from(Ident::new("STRATEGIES", Span::call_site())),
                                                                        })),
                                                                    })));
                                                                    args
                                                                },
                                                                gt_token: token::Gt::default(),
                                                            },
                                                        ),
                                                    });
                                                    segments
                                                },
                                            },
                                        }));
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
        ""
    }
}