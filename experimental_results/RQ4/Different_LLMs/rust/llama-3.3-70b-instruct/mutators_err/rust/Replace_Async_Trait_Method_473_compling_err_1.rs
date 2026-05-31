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

pub struct Replace_Async_Trait_Method_473;

impl Mutator for Replace_Async_Trait_Method_473 {
    fn name(&self) -> &str {
        "Replace_Async_Trait_Method_473"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                if type_impl_trait.bounds.iter().any(|bound| {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bound.path.segments.last().map_or(false, |segment| {
                                            segment.ident == "Future"
                                        })
                                    } else {
                                        false
                                    }
                                }) {
                                    // Replace async method call with synchronous method call
                                    func.sig.asyncness = None;
                                    for stmt in &mut func.block.stmts {
                                        if let Stmt::Expr(expr, _) = stmt {
                                            if let Expr::Await(await_expr) = &**expr {
                                                *expr = *await_expr.base.clone();
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
        "The mutation operator targets async trait methods and replaces the async method calls with synchronous method calls. This transformation aims to test the compiler's handling of async trait methods and may lead to ICEs or inference failures in the trait resolution system."
    }
}