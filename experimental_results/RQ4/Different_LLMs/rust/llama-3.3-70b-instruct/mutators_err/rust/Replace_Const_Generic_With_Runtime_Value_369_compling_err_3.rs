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

pub struct Replace_Const_Generic_With_Runtime_Value_369;

impl Mutator for Replace_Const_Generic_With_Runtime_Value_369 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Runtime_Value_369"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Array(array_type) = &*pat_type.ty {
                            if let Expr::Lit(lit) = array_type.len {
                                let new_type = parse_quote! { Vec<usize> };
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Array(array_type) = &*pat_type.ty {
                                    if let Expr::Lit(lit) = array_type.len {
                                        let new_type = parse_quote! { Vec<usize> };
                                        pat_type.ty = Box::new(new_type);
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
        "The mutation operator replaces const generic arguments with runtime values, potentially triggering bugs related to const generic expressions. It applies to any function or type with const generic arguments, replacing them with a runtime value, such as changing the function signature to accept the argument at runtime instead of compile time."
    }
}