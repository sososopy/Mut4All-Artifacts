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

pub struct Swap_Generic_Lifetime_Argument_341;

impl Mutator for Swap_Generic_Lifetime_Argument_341 {
    fn name(&self) -> &str {
        "Swap_Generic_Lifetime_Argument_341"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath {
                        path: SynPath { segments, .. },
                        ..
                    }) = &**return_type {
                        for segment in segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                let mut generic_args: Vec<GenericArgument> = args.args.iter().cloned().collect();
                                let mut lifetime_args: Vec<Lifetime> = func.sig.generics.lifetimes().map(|lt| lt.lifetime.clone()).collect();
                                if !generic_args.is_empty() && !lifetime_args.is_empty() {
                                    let generic_arg = generic_args.remove(0);
                                    let lifetime_arg = lifetime_args.remove(0);
                                    generic_args.insert(0, GenericArgument::Lifetime(lifetime_arg));
                                    args.args = generic_args;
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::Path(TypePath {
                                path: SynPath { segments, .. },
                                ..
                            }) = &**return_type {
                                for segment in segments {
                                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                        let mut generic_args: Vec<GenericArgument> = args.args.iter().cloned().collect();
                                        let mut lifetime_args: Vec<Lifetime> = func.sig.generics.lifetimes().map(|lt| lt.lifetime.clone()).collect();
                                        if !generic_args.is_empty() && !lifetime_args.is_empty() {
                                            let generic_arg = generic_args.remove(0);
                                            let lifetime_arg = lifetime_args.remove(0);
                                            generic_args.insert(0, GenericArgument::Lifetime(lifetime_arg));
                                            args.args = generic_args;
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
        "The mutation operator swaps a generic argument with a lifetime argument in a generic type, trait, or function definition. This transformation can help expose bugs in the Rust compiler related to generic const expressions and lifetime arguments."
    }
}