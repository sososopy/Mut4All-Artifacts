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

pub struct Replace_Const_With_Non_Const_315;

impl Mutator for Replace_Const_With_Non_Const_315 {
    fn name(&self) -> &str {
        "Replace_Const_With_Non_Const_315"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    func.sig.constness = None;
                }
            }

            if let syn::Item::Const(const_item) = item {
                let mut new_item = syn::Item::Fn(syn::ItemFn {
                    attrs: const_item.attrs.clone(),
                    vis: const_item.vis.clone(),
                    sig: syn::Signature {
                        constness: None,
                        asyncness: const_item.sig.asyncness.clone(),
                        unsafety: const_item.sig.unsafety.clone(),
                        ident: const_item.sig.ident.clone(),
                        generics: const_item.sig.generics.clone(),
                        inputs: const_item.sig.inputs.clone(),
                        variadic: const_item.sig.variadic.clone(),
                        output: const_item.sig.output.clone(),
                        paren_token: const_item.sig.paren_token.clone(),
                    },
                    block: syn::Block {
                        brace_token: const_item.block.brace_token.clone(),
                        stmts: {
                            let mut stmts = Punctuated::new();
                            stmts.push(syn::Stmt::Expr(
                                syn::Expr::Path(syn::ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: const_item.expr.clone(),
                                }),
                                None,
                            ));
                            stmts
                        },
                    },
                });
                *item = new_item;
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Const(const_item) = impl_item {
                        let mut new_item = syn::ImplItem::Fn(syn::ImplItemFn {
                            attrs: const_item.attrs.clone(),
                            vis: const_item.vis.clone(),
                            sig: syn::Signature {
                                constness: None,
                                asyncness: const_item.sig.asyncness.clone(),
                                unsafety: const_item.sig.unsafety.clone(),
                                ident: const_item.sig.ident.clone(),
                                generics: const_item.sig.generics.clone(),
                                inputs: const_item.sig.inputs.clone(),
                                variadic: const_item.sig.variadic.clone(),
                                output: const_item.sig.output.clone(),
                                paren_token: const_item.sig.paren_token.clone(),
                            },
                            block: syn::Block {
                                brace_token: const_item.block.brace_token.clone(),
                                stmts: {
                                    let mut stmts = Punctuated::new();
                                    stmts.push(syn::Stmt::Expr(
                                        syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: const_item.expr.clone(),
                                        }),
                                        None,
                                    ));
                                    stmts
                                },
                            },
                        });
                        *impl_item = new_item;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const functions and variables with their non-const counterparts to check for potential issues with const correctness and evaluate how the Rust compiler handles such changes."
    }
}