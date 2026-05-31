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

pub struct Replace_Constant_Type_59;

impl Mutator for Replace_Constant_Type_59 {
    fn name(&self) -> &str {
        "Replace_Constant_Type_59"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut constant_types: Vec<Type> = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for generic in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = generic {
                        if let Some(type_annotation) = item_struct.generics.type_params().find(|param| param.ident == type_param.ident) {
                            if let Some(default) = &type_annotation.default {
                                constant_types.push(default.clone());
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut generics = item_struct.generics.clone();
                let mut type_annotations: Vec<_> = generics.type_params().collect();
                for generic in &mut generics.params {
                    if let GenericParam::Type(type_param) = generic {
                        if let Some(type_annotation) = type_annotations.iter_mut().find(|param| param.ident == type_param.ident) {
                            let new_type = constant_types.choose(&mut thread_rng()).unwrap().clone();
                            if let Some(default) = &mut type_annotation.default {
                                *default = new_type.clone();
                            }
                        }
                    }
                }
                item_struct.generics = generics;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant types in struct definitions with other compatible types in the same scope, testing the compiler's ability to handle different type combinations and ensure correct type checking and constant evaluation."
    }
}