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

pub struct Replace_Default_Implementation_451;

impl Mutator for Replace_Default_Implementation_451 {
    fn name(&self) -> &str {
        "Replace_Default_Implementation_451"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        if let Some(default_impl) = &method.default {
                            let mut new_impl = default_impl.clone();
                            let mut rng = thread_rng();
                            let alternatives = vec![
                                parse_quote!(Vec<Self>),
                                parse_quote!(Option<Self>),
                                parse_quote!(Box<Self>),
                            ];
                            let alternative = alternatives.choose(&mut rng).unwrap();
                            new_impl.block.stmts.clear();
                            new_impl.block.stmts.push(Stmt::Expr(
                                Expr::Path(ExprPath {
                                    qself: None,
                                    attrs: vec![],
                                    path: alternative.clone(),
                                }),
                                None,
                            ));
                            method.default = Some(new_impl);
                        }
                    } else if let syn::TraitItem::Method(method) = item.clone() {
                        if let Some(default_impl) = &method.default {
                            let mut new_impl = default_impl.clone();
                            let mut rng = thread_rng();
                            let alternatives = vec![
                                parse_quote!(Vec<Self>),
                                parse_quote!(Option<Self>),
                                parse_quote!(Box<Self>),
                            ];
                            let alternative = alternatives.choose(&mut rng).unwrap();
                            new_impl.block.stmts.clear();
                            new_impl.block.stmts.push(Stmt::Expr(
                                Expr::Path(ExprPath {
                                    qself: None,
                                    attrs: vec![],
                                    path: alternative.clone(),
                                }),
                                None,
                            ));
                            trait_item.items.iter_mut().for_each(|x| {
                                if let syn::TraitItem::Method(m) = x {
                                    if m.sig.ident == method.sig.ident {
                                        m.default = Some(new_impl.clone());
                                    }
                                }
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits with default implementations for associated types. It replaces the default implementation with an alternative type that could logically serve the same purpose, testing the compiler's ability to handle different types in the context of trait specialization and associated types."
    }
}

impl Replace_Default_Implementation_451 {
    pub fn new() -> Self {
        Replace_Default_Implementation_451
    }
}