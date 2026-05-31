use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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
            if let Item::Struct(item_struct) = item {
                let mut has_const_generics = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generics = true;
                        break;
                    }
                }
                if !has_const_generics {
                    continue;
                }
                let mut new_generics = item_struct.generics.clone();
                new_generics.params.clear();
                for param in &item_struct.generics.params {
                    match param {
                        GenericParam::Const(const_param) => {
                            let mut new_const_param = const_param.clone();
                            if let Type::Path(type_path) = &const_param.ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "i64" {
                                        new_const_param.ty = parse_quote!(usize);
                                    } else if last_segment.ident == "usize" {
                                        new_const_param.ty = parse_quote!(i64);
                                    }
                                }
                            }
                            new_generics
                                .params
                                .push(GenericParam::Const(new_const_param));
                        }
                        _ => {
                            new_generics.params.push(param.clone());
                        }
                    }
                }
                item_struct.generics = new_generics;
            }
            if let Item::Impl(item_impl) = item {
                let mut has_const_generics = false;
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generics = true;
                        break;
                    }
                }
                if !has_const_generics {
                    continue;
                }
                let mut new_generics = item_impl.generics.clone();
                new_generics.params.clear();
                for param in &item_impl.generics.params {
                    match param {
                        GenericParam::Const(const_param) => {
                            let mut new_const_param = const_param.clone();
                            if let Type::Path(type_path) = &const_param.ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "i64" {
                                        new_const_param.ty = parse_quote!(usize);
                                    } else if last_segment.ident == "usize" {
                                        new_const_param.ty = parse_quote!(i64);
                                    }
                                }
                            }
                            new_generics
                                .params
                                .push(GenericParam::Const(new_const_param));
                        }
                        _ => {
                            new_generics.params.push(param.clone());
                        }
                    }
                }
                item_impl.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs and impls with const generics, specifically altering the types of const generics between i64 and usize. By modifying the const generic types, it aims to provoke layout mismatches or type inference issues, particularly those that could lead to internal compiler errors related to layout inconsistencies."
    }
}