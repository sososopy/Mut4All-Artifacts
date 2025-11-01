use crate::mutator::Mutator;
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
pub struct Insert_Self_Invocation_4;
impl Mutator for Insert_Self_Invocation_4 {
    fn name(&self) -> &str {
        "Insert_Self_Invocation_4 "
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main " {
                    continue;
                }
                let fn_name = item_fn.sig.ident.clone();
                let args: Vec<_> = item_fn
                    .sig
                    .inputs
                    .iter()
                    .filter_map(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            Some(pat_type.pat.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                let call = Expr::Call(ExprCall {
                    attrs: vec![],
                    func: Box::new(Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: fn_name.into(),
                    })),
                    paren_token: Default::default(),
                    args: args
                        .into_iter()
                        .map(|pat| {
                            if let Pat::Ident(pat_ident) = *pat {
                                Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path::from(pat_ident.ident),
                                })
                            } else {
                                Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: syn::Path::from(syn::Ident::new(
                                        "dummy ",
                                        proc_macro2::Span::call_site(),
                                    )),
                                })
                            }
                        })
                        .collect(),
                });
                item_fn
                    .block
                    .stmts
                    .insert(0, Stmt::Expr(call, Some(syn::token::Semi::default())));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main " {
                            continue;
                        }
                        let fn_name = method.sig.ident.clone();
                        let args: Vec<_> = method
                            .sig
                            .inputs
                            .iter()
                            .filter_map(|arg| {
                                if let syn::FnArg::Typed(pat_type) = arg {
                                    Some(pat_type.pat.clone())
                                } else {
                                    None
                                }
                            })
                            .collect();
                        let call = syn::Expr::Call(syn::ExprCall {
                            attrs: vec![],
                            func: Box::new(syn::Expr::Path(syn::ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: fn_name.into(),
                            })),
                            paren_token: Default::default(),
                            args: args
                                .into_iter()
                                .map(|pat| {
                                    if let syn::Pat::Ident(pat_ident) = *pat {
                                        syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: syn::Path::from(pat_ident.ident),
                                        })
                                    } else {
                                        syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: syn::Path::from(syn::Ident::new(
                                                "dummy ",
                                                proc_macro2::Span::call_site(),
                                            )),
                                        })
                                    }
                                })
                                .collect(),
                        });
                        method
                            .block
                            .stmts
                            .insert(0, syn::Stmt::Expr(call, Some(syn::token::Semi::default())));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all non-main functions, including both free functions and methods within impl blocks. It inserts a call to the current function itself at the beginning of its body, using the original parameters as arguments. This transformation is intended to induce recursive behavior, increasing the likelihood of triggering compiler errors related to recursion, borrow checking, parameter reuse, and code generation. It is particularly useful for detecting edge-case issues in control flow analysis and runtime execution safety. "
    }
}
