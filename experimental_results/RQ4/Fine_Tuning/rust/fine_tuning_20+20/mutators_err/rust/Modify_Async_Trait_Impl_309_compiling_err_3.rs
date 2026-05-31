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

pub struct Modify_Async_Trait_Impl_309;

impl Mutator for Modify_Async_Trait_Impl_309 {
    fn name(&self) -> &str {
        "Modify_Async_Trait_Impl_309"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.iter().any(|seg| {
                        if let PathArguments::None = seg.arguments {
                            true
                        } else {
                            false
                        }
                    }) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if method.sig.asyncness.is_some() {
                                    method.sig.asyncness = None;
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Item(Item::Fn(inner_fn)) = stmt {
                                            if inner_fn.sig.asyncness.is_some() {
                                                let new_inner_fn = Item::Fn(ItemFn {
                                                    attrs: inner_fn.attrs.clone(),
                                                    vis: inner_fn.vis.clone(),
                                                    sig: syn::Signature {
                                                        constness: inner_fn.sig.constness,
                                                        asyncness: None,
                                                        unsafety: inner_fn.sig.unsafety,
                                                        abi: inner_fn.sig.abi.clone(),
                                                        fn_token: inner_fn.sig.fn_token,
                                                        ident: inner_fn.sig.ident.clone(),
                                                        generics: inner_fn.sig.generics.clone(),
                                                        paren_token: inner_fn.sig.paren_token,
                                                        inputs: inner_fn.sig.inputs.clone(),
                                                        variadic: inner_fn.sig.variadic.clone(),
                                                        output: inner_fn.sig.output.clone(),
                                                    },
                                                    block: inner_fn.block.clone(),
                                                });
                                                new_stmts.push(Stmt::Item(new_inner_fn));
                                            } else {
                                                new_stmts.push(Stmt::Item(
                                                    inner_fn.clone(),
                                                ));
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that include async functions. It transforms async functions within the impl block into non-async functions by removing the `async` keyword and adjusting the function body to be synchronous. This mutation exploits the differences in how async and non-async functions are handled within trait implementations, potentially revealing compiler bugs related to async function resolution and execution in trait contexts."
    }
}