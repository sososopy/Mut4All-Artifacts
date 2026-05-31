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

pub struct ConstGenericParameterTypeSubstitution_94;

impl Mutator for ConstGenericParameterTypeSubstitution_94 {
    fn name(&self) -> &str {
        "ConstGenericParameterTypeSubstitution_94"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = HashSet::new();
        concrete_types.insert("usize");
        concrete_types.insert("i32");
        concrete_types.insert("u32");
        concrete_types.insert("i64");
        concrete_types.insert("u64");
        concrete_types.insert("bool");
        concrete_types.insert("char");
        concrete_types.insert("f32");
        concrete_types.insert("f64");

        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let ident = &type_path.path.segments[0].ident;
                                if ident.to_string().chars().next().unwrap().is_uppercase() {
                                    let mut rng = thread_rng();
                                    let concrete_type = concrete_types.iter().collect::<Vec<_>>()[rng.gen_range(0..concrete_types.len())];
                                    const_param.ty = parse_quote!(#concrete_type);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Fn(item_fn) = item {
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let ident = &type_path.path.segments[0].ident;
                                if ident.to_string().chars().next().unwrap().is_uppercase() {
                                    let mut rng = thread_rng();
                                    let concrete_type = concrete_types.iter().collect::<Vec<_>>()[rng.gen_range(0..concrete_types.len())];
                                    const_param.ty = parse_quote!(#concrete_type);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let ident = &type_path.path.segments[0].ident;
                                if ident.to_string().chars().next().unwrap().is_uppercase() {
                                    let mut rng = thread_rng();
                                    let concrete_type = concrete_types.iter().collect::<Vec<_>>()[rng.gen_range(0..concrete_types.len())];
                                    const_param.ty = parse_quote!(#concrete_type);
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Type::Path(type_path) = &const_param.ty {
                                    if type_path.path.segments.len() == 1 {
                                        let ident = &type_path.path.segments[0].ident;
                                        if ident.to_string().chars().next().unwrap().is_uppercase() {
                                            let mut rng = thread_rng();
                                            let concrete_type = concrete_types.iter().collect::<Vec<_>>()[rng.gen_range(0..concrete_types.len())];
                                            const_param.ty = parse_quote!(#concrete_type);
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
        "The mutation operator targets const generic parameter declarations where the type is a generic type parameter (e.g., `const N: T`). It substitutes the generic type with a concrete primitive type (e.g., `usize`, `i32`) randomly chosen from a predefined set. This transformation aims to expose compiler bugs related to const generic type mismatches, type resolution, and equality checks, potentially triggering ICEs or unexpected behavior in generic contexts."
    }
}