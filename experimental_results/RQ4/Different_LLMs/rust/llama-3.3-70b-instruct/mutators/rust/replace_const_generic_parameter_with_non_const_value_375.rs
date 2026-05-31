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

pub struct Replace_Const_Generic_Parameter_With_Non_Const_Value_375;

impl Mutator for Replace_Const_Generic_Parameter_With_Non_Const_Value_375 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameter_With_Non_Const_Value_375"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    continue;
                }
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_value = match const_param.ty {
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: ref segments,
                                },
                            }) => {
                                let segment = segments.first().unwrap();
                                let ident = segment.ident.clone();
                                match ident.to_string().as_str() {
                                    "i32" => {
                                        let value: i32 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "i64" => {
                                        let value: i64 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "usize" => {
                                        let value: usize = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "isize" => {
                                        let value: isize = thread_rng().gen_range(-100..100);
                                        quote! { #value }
                                    }
                                    "bool" => {
                                        let value: bool = thread_rng().gen_bool(0.5);
                                        quote! { #value }
                                    }
                                    "char" => {
                                        let value: char =
                                            thread_rng().gen_range('a'..='z');
                                        quote! { #value }
                                    }
                                    _ => {
                                        let value: i32 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                }
                            }
                            _ => {
                                let value: i32 = thread_rng().gen_range(0..100);
                                quote! { #value }
                            }
                        };
                        const_param.default = Some(syn::Expr::Verbatim(new_value));
                    } else {
                        continue;
                    }
                }
            }
            if let syn::Item::Enum(enum_item) = item {
                if enum_item.generics.params.is_empty() {
                    continue;
                }
                for param in &mut enum_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_value = match const_param.ty {
                            Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: ref segments,
                                },
                            }) => {
                                let segment = segments.first().unwrap();
                                let ident = segment.ident.clone();
                                match ident.to_string().as_str() {
                                    "i32" => {
                                        let value: i32 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "i64" => {
                                        let value: i64 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "usize" => {
                                        let value: usize = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                    "isize" => {
                                        let value: isize = thread_rng().gen_range(-100..100);
                                        quote! { #value }
                                    }
                                    "bool" => {
                                        let value: bool = thread_rng().gen_bool(0.5);
                                        quote! { #value }
                                    }
                                    "char" => {
                                        let value: char =
                                            thread_rng().gen_range('a'..='z');
                                        quote! { #value }
                                    }
                                    _ => {
                                        let value: i32 = thread_rng().gen_range(0..100);
                                        quote! { #value }
                                    }
                                }
                            }
                            _ => {
                                let value: i32 = thread_rng().gen_range(0..100);
                                quote! { #value }
                            }
                        };
                        const_param.default = Some(syn::Expr::Verbatim(new_value));
                    } else {
                        continue;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const values of the same type, testing how the compiler handles cases where const generic parameters are used in a non-const context."
    }
}