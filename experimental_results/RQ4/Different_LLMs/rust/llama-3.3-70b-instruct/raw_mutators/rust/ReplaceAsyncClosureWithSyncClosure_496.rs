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
                        if let Type::Closure(closure) = &*pat_type.ty {
                            if closure.asyncness.is_some() {
                                let mut new_closure = closure.clone();
                                new_closure.asyncness = None;
                                pat_type.ty = Box::new(Type::Closure(new_closure));
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
                                if let Type::Closure(closure) = &*pat_type.ty {
                                    if closure.asyncness.is_some() {
                                        let mut new_closure = closure.clone();
                                        new_closure.asyncness = None;
                                        pat_type.ty = Box::new(Type::Closure(new_closure));
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