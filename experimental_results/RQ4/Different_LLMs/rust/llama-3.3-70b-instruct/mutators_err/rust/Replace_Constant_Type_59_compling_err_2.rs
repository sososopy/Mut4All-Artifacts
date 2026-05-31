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
                    if let GenericParam::Const(_) = generic {
                        if let GenericParam::Type(type_param) = generic {
                            if let Some(type_annotation) = item_struct.generics.type_params().find(|param| param.ident == type_param.ident) {
                                constant_types.push(type_annotation.default().unwrap().clone());
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for generic in &mut item_struct.generics.params {
                    if let GenericParam::Const(_) = generic {
                        if let GenericParam::Type(type_param) = generic {
                            if let Some(type_annotation) = item_struct.generics.type_params().find(|param| param.ident == type_param.ident) {
                                let new_type = constant_types.choose(&mut thread_rng()).unwrap().clone();
                                *type_annotation.default_mut().unwrap() = new_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant types in struct definitions with other compatible types in the same scope, testing the compiler's ability to handle different type combinations and ensure correct type checking and constant evaluation."
    }
}