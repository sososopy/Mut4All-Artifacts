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

pub struct ReplaceAsyncClosureWithSyncClosure_496;

impl Mutator for ReplaceAsyncClosureWithSyncClosure_496 {
    fn name(&self) -> &str {
        "ReplaceAsyncClosureWithSyncClosure_496"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::ImplTrait(impl_trait) = &*pat_type.ty {
                            if let Some(bounds) = impl_trait.bounds.as_ref() {
                                if bounds.len() > 0 {
                                    let mut new_impl_trait = impl_trait.clone();
                                    new_impl_trait.bounds = Punctuated::new();
                                    pat_type.ty = Box::new(Type::ImplTrait(new_impl_trait));
                                }
                            }
                        } else if let Type::Path(path) = &*pat_type.ty {
                            if let Some(segment) = path.path.segments.last() {
                                if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                    if let Some(arguments) = segment.arguments.as_ref() {
                                        if let PathArguments::AngleBracketed(angle_bracketed) = arguments {
                                            if angle_bracketed.args.len() > 0 {
                                                let mut new_path = path.clone();
                                                new_path.path.segments.last_mut().unwrap().arguments = None;
                                                pat_type.ty = Box::new(Type::Path(new_path));
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
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::ImplTrait(impl_trait) = &*pat_type.ty {
                                    if let Some(bounds) = impl_trait.bounds.as_ref() {
                                        if bounds.len() > 0 {
                                            let mut new_impl_trait = impl_trait.clone();
                                            new_impl_trait.bounds = Punctuated::new();
                                            pat_type.ty = Box::new(Type::ImplTrait(new_impl_trait));
                                        }
                                    }
                                } else if let Type::Path(path) = &*pat_type.ty {
                                    if let Some(segment) = path.path.segments.last() {
                                        if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                            if let Some(arguments) = segment.arguments.as_ref() {
                                                if let PathArguments::AngleBracketed(angle_bracketed) = arguments {
                                                    if angle_bracketed.args.len() > 0 {
                                                        let mut new_path = path.clone();
                                                        new_path.path.segments.last_mut().unwrap().arguments = None;
                                                        pat_type.ty = Box::new(Type::Path(new_path));
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
        "The mutation operator replaces async closures with sync closures in function and method signatures. By removing the async keyword from closure definitions, it tests the compiler's handling of closure types and async/await syntax, potentially leading to errors or unexpected behavior in the presence of unboxed closures and async functions."
    }
}