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

pub struct Replace_Generic_Constants_With_Other_Constants_91;

impl Mutator for Replace_Generic_Constants_With_Other_Constants_91 {
    fn name(&self) -> &str {
        "Replace_Generic_Constants_With_Other_Constants_91"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let syn::Expr::Path(path) = &*const_item.expr {
                    if let Some(constant) = get_constant_value(&path.path) {
                        let new_constant = get_new_constant(constant);
                        const_item.expr = parse_quote! { #new_constant };
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(constant) = get_constant_value(&type_path.path) {
                                let new_constant = get_new_constant(constant);
                                pat_type.ty = parse_quote! { #new_constant };
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
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(constant) = get_constant_value(&type_path.path) {
                                        let new_constant = get_new_constant(constant);
                                        pat_type.ty = parse_quote! { #new_constant };
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
        "The mutation operator replaces generic constants with other constants of the same type. It aims to test the compiler's handling of different constant values and identify potential bugs in the compiler's implementation of generic constants and constant expressions."
    }
}

fn get_constant_value(path: &SynPath) -> Option<i32> {
    if let Some(segment) = path.segments.get(0) {
        if let Some(ident_str) = segment.ident.to_string().parse::<i32>().ok() {
            return Some(ident_str);
        }
    }
    None
}

fn get_new_constant(constant: i32) -> i32 {
    let mut rng = thread_rng();
    let new_constant = rng.gen_range(0..100);
    new_constant
}