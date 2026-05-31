use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprLit, ExprPath, File, GenericArgument, GenericParam, Ident,
    Item, ItemStruct, Lit, Path as SynPath, PathArguments, Type, TypePath, parse_quote,
    punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Parameters_361;

impl Mutator for Modify_Const_Generic_Parameters_361 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_361"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    if let syn::Type::Path(TypePath { path, .. }) = &generics.ty {
                        if let Some(segment) = path.segments.last() {
                            if segment.ident == "usize" {
                                if let Some(default) = &generics.default {
                                    if let syn::Expr::Path(ExprPath { path, .. }) = &default {
                                        if let Some(segment) = path.segments.last() {
                                            let new_expr: Expr = parse_quote! {
                                                #segment + 1
                                            };
                                            generics.default = Some(new_expr);
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
        ""
    }
}