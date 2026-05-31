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

pub struct Modify_Const_Generics_In_Structs_399;

impl Mutator for Modify_Const_Generics_In_Structs_399 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Structs_399"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.iter_mut().filter_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if let syn::Type::Path(type_path) = const_param.ty.as_ref() {
                            if let Some(segment) = type_path.path.segments.first() {
                                if segment.ident == "i64" {
                                    return Some(const_param);
                                }
                            }
                        }
                    }
                    None
                }).next() {
                    generics.ty = parse_quote!(usize);
                }
            }

            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let syn::Type::Path(type_path) = const_param.ty.as_ref() {
                                    if let Some(segment) = type_path.path.segments.first() {
                                        if segment.ident == "i64" {
                                            const_param.ty = parse_quote!(usize);
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
        "This mutation operator targets structs and their implementations using const generics with integer types. It modifies the type from i64 to usize, ensuring consistency across struct definitions and impl blocks. This change aims to explore potential issues with type mismatches and layout discrepancies, increasing the likelihood of encountering related compiler bugs."
    }
}