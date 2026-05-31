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

pub struct Replace_Async_Trait_Method_With_Impl_Future_306;

impl Mutator for Replace_Async_Trait_Method_With_Impl_Future_306 {
    fn name(&self) -> &str {
        "Replace_Async_Trait_Method_With_Impl_Future_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_future_import = false;
        for item in &file.items {
            if let Item::Use(use_item) = item {
                if let UseTree::Path(use_path) = &use_item.tree {
                    if let Some(seg) = use_path.path.segments.last() {
                        if seg.ident == "Future" {
                            has_future_import = true;
                            break;
                        }
                    }
                }
            }
        }
        if !has_future_import {
            let future_use: Item = parse_quote! { use core::future::Future; };
            file.items.insert(0, future_use);
        }
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(trait_fn) = trait_item {
                        if trait_fn.sig.asyncness.is_some() {
                            let return_type = if let ReturnType::Type(_, ty) = &trait_fn.sig.output {
                                ty.clone()
                            } else {
                                Box::new(Type::Tuple(TypeTuple {
                                    paren_token: token::Paren::default(),
                                    elems: Punctuated::new(),
                                }))
                            };
                            trait_fn.sig.asyncness = None;
                            trait_fn.sig.output = parse_quote! { -> impl Future<Output = #return_type> };
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(impl_fn) = impl_item {
                        if impl_fn.sig.asyncness.is_some() {
                            let return_type = if let ReturnType::Type(_, ty) = &impl_fn.sig.output {
                                ty.clone()
                            } else {
                                Box::new(Type::Tuple(TypeTuple {
                                    paren_token: token::Paren::default(),
                                    elems: Punctuated::new(),
                                }))
                            };
                            impl_fn.sig.asyncness = None;
                            impl_fn.sig.output = parse_quote! { -> impl Future<Output = #return_type> };
                            let block = impl_fn.block.clone();
                            impl_fn.block = parse_quote! { { async move #block } };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms async trait methods into non-async methods returning `impl Future<Output = ...>`. It modifies both trait definitions and implementations, ensuring the Future trait is in scope. This transformation tests the compiler's handling of async trait desugaring, future compatibility, and trait resolution for manually implemented futures, potentially exposing edge cases in async/await and trait system interactions."
    }
}