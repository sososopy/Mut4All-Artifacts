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

pub struct Replace_Mutable_Reference_With_Const_Reference_60;

impl Mutator for Replace_Mutable_Reference_With_Const_Reference_60 {
    fn name(&self) -> &str {
        "Replace_Mutable_Reference_With_Const_Reference_60"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let syn::Expr::Reference(reference) = &*const_item.expr {
                    if reference.mutability.is_some() {
                        let new_reference = syn::Expr::Reference(syn::ExprReference {
                            attrs: reference.attrs.clone(),
                            and_token: reference.and_token,
                            mutability: None,
                            expr: Box::new(*reference.expr.clone()),
                        });
                        const_item.expr = Box::new(new_reference);
                    }
                }
            }

            if let syn::Item::Static(static_item) = item {
                if let syn::Expr::Reference(reference) = &*static_item.expr {
                    if reference.mutability.is_some() {
                        let new_reference = syn::Expr::Reference(syn::ExprReference {
                            attrs: reference.attrs.clone(),
                            and_token: reference.and_token,
                            mutability: None,
                            expr: Box::new(*reference.expr.clone()),
                        });
                        static_item.expr = Box::new(new_reference);
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Reference(reference) = &*pat_type.ty {
                            if reference.mutability.is_some() {
                                let new_reference = Type::Reference(syn::TypeReference {
                                    and_token: reference.and_token,
                                    lifetime: reference.lifetime.clone(),
                                    mutability: None,
                                    elem: Box::new(*reference.elem.clone()),
                                });
                                pat_type.ty = Box::new(new_reference);
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Reference(reference) = &*pat_type.ty {
                                    if reference.mutability.is_some() {
                                        let new_reference = Type::Reference(syn::TypeReference {
                                            and_token: reference.and_token,
                                            lifetime: reference.lifetime.clone(),
                                            mutability: None,
                                            elem: Box::new(*reference.elem.clone()),
                                        });
                                        pat_type.ty = Box::new(new_reference);
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
        "The mutation operator replaces mutable references with constant references in constant declarations, static variables, function parameters, and return types. This transformation aims to test the compiler's handling of mutable references in constant contexts and may lead to ICEs or inference failures in the const-eval feature."
    }
}