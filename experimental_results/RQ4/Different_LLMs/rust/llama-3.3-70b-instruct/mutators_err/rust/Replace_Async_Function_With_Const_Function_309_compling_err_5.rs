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

pub struct Replace_Async_Function_With_Const_Function_309;

impl Mutator for Replace_Async_Function_With_Const_Function_309 {
    fn name(&self) -> &str {
        "Replace_Async_Function_With_Const_Function_309"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        if method.sig.asyncness.is_some() {
                            method.sig.asyncness = None;
                            method.sig.constness = Some(token::Const {
                                span: Span::call_site(),
                            });
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Method(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                            func.sig.constness = Some(token::Const {
                                span: Span::call_site(),
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions in traits with const functions to trigger bugs related to the interaction between async functions and const trait implementations."
    }
}

impl Replace_Async_Function_With_Const_Function_309 {
    fn mutate_trait_item(&self, trait_item: &mut syn::TraitItem) {
        if let syn::TraitItem::Method(method) = trait_item {
            if method.sig.asyncness.is_some() {
                method.sig.asyncness = None;
                method.sig.constness = Some(token::Const {
                    span: Span::call_site(),
                });
            }
        }
    }

    fn mutate_impl_item(&self, impl_item: &mut syn::ImplItem) {
        if let syn::ImplItem::Method(func) = impl_item {
            if func.sig.asyncness.is_some() {
                func.sig.asyncness = None;
                func.sig.constness = Some(token::Const {
                    span: Span::call_site(),
                });
            }
        }
    }
}

impl VisitMut for Replace_Async_Function_With_Const_Function_309 {
    fn visit_trait_item_mut(&mut self, i: &mut syn::TraitItem) {
        self.mutate_trait_item(i);
        visit_mut::visit_trait_item_mut(self, i);
    }

    fn visit_impl_item_mut(&mut self, i: &mut syn::ImplItem) {
        self.mutate_impl_item(i);
        visit_mut::visit_impl_item_mut(self, i);
    }
}

impl Replace_Async_Function_With_Const_Function_309 {
    fn mutate(&mut self, file: &mut syn::File) {
        visit_mut::visit_file_mut(self, file);
    }
}