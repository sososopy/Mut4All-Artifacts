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

pub struct Replace_Type_Annotation_468;

impl Mutator for Replace_Type_Annotation_468 {
    fn name(&self) -> &str {
        "Replace_Type_Annotation_468"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types: Vec<Type> = Vec::new();

        // Collect existing types in the file
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                for field in &struct_item.fields {
                    if let Some(field_ty) = &field.ty {
                        existing_types.push(field_ty.clone());
                    }
                }
            }
            if let Item::Fn(func) = item {
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        existing_types.push(*pat_type.ty.clone());
                    }
                }
                if let ReturnType::Type(_, return_ty) = &func.sig.output {
                    existing_types.push(*return_ty.clone());
                }
            }
        }

        // Replace type annotations
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Some(field_ty) = &mut field.ty {
                        if !existing_types.is_empty() {
                            let replacement_ty = existing_types.choose(&mut thread_rng()).unwrap().clone();
                            *field_ty = replacement_ty;
                        }
                    }
                }
            }
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if !existing_types.is_empty() {
                            let replacement_ty = existing_types.choose(&mut thread_rng()).unwrap().clone();
                            *pat_type.ty = Box::new(replacement_ty);
                        }
                    }
                }
                if let ReturnType::Type(_, return_ty) = &mut func.sig.output {
                    if !existing_types.is_empty() {
                        let replacement_ty = existing_types.choose(&mut thread_rng()).unwrap().clone();
                        *return_ty = Box::new(replacement_ty);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type annotations in the given Rust code with different type annotations. It targets type annotations for variables, function parameters, and return types. The replacement type is chosen based on the existing types in the seed program to maintain consistency and avoid introducing ambiguous or invalid types."
    }
}