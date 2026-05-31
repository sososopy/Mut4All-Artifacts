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

pub struct Replace_Scalar_Type_With_Pointer_Type_227;

impl Mutator for Replace_Scalar_Type_With_Pointer_Type_227 {
    fn name(&self) -> &str {
        "Replace_Scalar_Type_With_Pointer_Type_227"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let Some(ident) = &segment.ident {
                                    let ident_str = ident.to_string();
                                    if ident_str == "i32" || ident_str == "i64" || ident_str == "bool" {
                                        let new_type = parse_quote!{*mut #ident};
                                        pat_type.ty = Box::new(new_type);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Static(item_static) = item {
                if let Some(type_path) = &item_static.ty {
                    if let Type::Path(path) = type_path {
                        if let Some(segment) = path.path.segments.last() {
                            if let Some(ident) = &segment.ident {
                                let ident_str = ident.to_string();
                                if ident_str == "i32" || ident_str == "i64" || ident_str == "bool" {
                                    let new_type = parse_quote!{*mut #ident};
                                    item_static.ty = Some(Box::new(new_type));
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Const(item_const) = item {
                if let Some(type_path) = &item_const.ty {
                    if let Type::Path(path) = type_path {
                        if let Some(segment) = path.path.segments.last() {
                            if let Some(ident) = &segment.ident {
                                let ident_str = ident.to_string();
                                if ident_str == "i32" || ident_str == "i64" || ident_str == "bool" {
                                    let new_type = parse_quote!{*mut #ident};
                                    item_const.ty = Some(Box::new(new_type));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces scalar types with pointer types to trigger bugs related to type mismatches. It targets expressions containing scalar types, such as integers or booleans, in functions or global variables, and changes them to mutable pointer types."
    }
}