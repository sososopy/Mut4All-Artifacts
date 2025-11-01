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

pub struct Modify_Const_Generics_200;

impl Mutator for Modify_Const_Generics_200 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_200"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.iter_mut().filter_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }).collect::<Vec<_>>() {
                    for const_param in generics {
                        if let Type::Path(type_path) = &mut const_param.ty {
                            if let Some(segment) = type_path.path.segments.first_mut() {
                                if segment.ident == "i64" {
                                    segment.ident = Ident::new("usize", Span::call_site());
                                } else if segment.ident == "usize" {
                                    segment.ident = Ident::new("i64", Span::call_site());
                                }
                            }
                        }
                    }
                }
            } else if let Item::Impl(item_impl) = item {
                for const_param in &mut item_impl.generics.params {
                    if let GenericParam::Const(const_param) = const_param {
                        if let Type::Path(type_path) = &mut const_param.ty {
                            if let Some(segment) = type_path.path.segments.first_mut() {
                                if segment.ident == "i64" {
                                    segment.ident = Ident::new("usize", Span::call_site());
                                } else if segment.ident == "usize" {
                                    segment.ident = Ident::new("i64", Span::call_site());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs utilizing const generics, specifically altering the types of these generics to explore potential type mismatches. It identifies structs and their implementation blocks, modifying const generics from i64 to usize or vice versa, ensuring consistency across the code."
    }
}