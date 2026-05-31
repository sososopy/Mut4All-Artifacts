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

pub struct Swap_Closure_Lifetimes_40;

impl Mutator for Swap_Closure_Lifetimes_40 {
    fn name(&self) -> &str {
        "Swap_Closure_Lifetimes_40"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.mutate_closure_lifetimes(func);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.mutate_closure_lifetimes(method);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the lifetime parameters of nested closures. This transformation helps expose bugs in the Rust compiler related to lifetime handling and closure normalization."
    }
}

impl Swap_Closure_Lifetimes_40 {
    fn mutate_closure_lifetimes(&self, func: &mut syn::ItemFn) {
        let mut lifetimes: Vec<Lifetime> = Vec::new();
        for param in &func.sig.generics.params {
            if let GenericParam::Lifetime(lifetime) = param {
                lifetimes.push(lifetime.lifetime.clone());
            }
        }

        for stmt in &mut func.block.stmts {
            if let Stmt::Expr(expr) = stmt {
                if let Expr::Closure(closure) = &mut **expr {
                    let mut closure_lifetimes: Vec<Lifetime> = Vec::new();
                    for param in &closure.sig.generics.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            closure_lifetimes.push(lifetime.lifetime.clone());
                        }
                    }

                    if closure_lifetimes.len() > 1 {
                        closure_lifetimes.reverse();
                        for param in &mut closure.sig.generics.params {
                            if let GenericParam::Lifetime(lifetime) = param {
                                lifetime.lifetime = closure_lifetimes.remove(0);
                            }
                        }
                    }
                }
            }
        }
    }
}