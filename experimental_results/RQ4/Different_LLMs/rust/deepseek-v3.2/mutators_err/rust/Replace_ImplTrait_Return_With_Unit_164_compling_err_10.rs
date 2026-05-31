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

pub struct Replace_ImplTrait_Return_With_Unit_164;

impl Mutator for Replace_ImplTrait_Return_With_Unit_164 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_Unit_164"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        *return_type = Box::new(Type::Tuple(syn::TypeTuple {
                            paren_token: token::Paren::default(),
                            elems: Punctuated::new(),
                        }));
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(item_fn) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                            if let Type::ImplTrait(_) = &**return_type {
                                *return_type = Box::new(Type::Tuple(syn::TypeTuple {
                                    paren_token: token::Paren::default(),
                                    elems: Punctuated::new(),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the return type of functions and methods returning `impl Trait` (including nested `impl Trait` in associated bounds) with an empty tuple `()`. This transformation changes only the return type annotation, leaving the body unchanged. It targets opaque `impl Trait` returns, potentially triggering type unification and inference errors, especially when the body's actual type mismatches the mutated return type, which can expose internal compiler errors."
    }
}