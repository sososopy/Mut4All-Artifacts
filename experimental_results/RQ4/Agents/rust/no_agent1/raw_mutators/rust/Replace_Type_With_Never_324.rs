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

pub struct Replace_Type_With_Never_324;

impl Mutator for Replace_Type_With_Never_324 {
    fn name(&self) -> &str {
        "Replace_Type_With_Never_324"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        pat_type.ty = Box::new(parse_quote!(!));
                    }
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(parse_quote!(!));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                pat_type.ty = Box::new(parse_quote!(!));
                            }
                        }
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            *ty = Box::new(parse_quote!(!));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces all parameter and return types in non-main functions and methods with the never type `!`. This transformation introduces uninhabited types, challenging the compiler's type checking and control flow analysis. It aims to uncover deep compiler bugs related to type inference, unreachable code, and type normalization."
    }
}