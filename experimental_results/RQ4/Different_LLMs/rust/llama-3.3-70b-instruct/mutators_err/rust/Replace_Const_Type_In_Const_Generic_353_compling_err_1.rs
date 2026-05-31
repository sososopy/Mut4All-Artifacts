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

pub struct Replace_Const_Type_In_Const_Generic_353;

impl Mutator for Replace_Const_Type_In_Const_Generic_353 {
    fn name(&self) -> &str {
        "Replace_Const_Type_In_Const_Generic_353"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let syn::Expr::Path(expr_path) = &const_item.expr {
                    if let Some(const_type) = get_const_type(const_item) {
                        let new_type = get_replacement_type(const_type);
                        if let Some(new_type) = new_type {
                            replace_const_type(const_item, new_type);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the type of a constant in a const generic with a different type that has the same size and alignment. This transformation checks if the compiler correctly handles the change in type and can help expose bugs related to type checking and substitution in const generics."
    }
}

fn get_const_type(const_item: &syn::ItemConst) -> Option<&syn::Type> {
    if let syn::Expr::Path(expr_path) = &const_item.expr {
        if let Some(const_type) = expr_path.path.segments.last() {
            if let syn::PathArguments::None = const_type.arguments {
                return Some(&const_item.ty);
            }
        }
    }
    None
}

fn get_replacement_type(const_type: &syn::Type) -> Option<syn::Type> {
    match const_type {
        syn::Type::Path(type_path) => {
            if type_path.path.segments.last().unwrap().ident == "u32" {
                Some(syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                            ident: Ident::new("i32", Span::call_site()),
                            arguments: syn::PathArguments::None,
                        }]),
                    },
                }))
            } else if type_path.path.segments.last().unwrap().ident == "i32" {
                Some(syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                            ident: Ident::new("u32", Span::call_site()),
                            arguments: syn::PathArguments::None,
                        }]),
                    },
                }))
            } else {
                None
            }
        }
        _ => None,
    }
}

fn replace_const_type(const_item: &mut syn::ItemConst, new_type: syn::Type) {
    const_item.ty = Box::new(new_type);
}