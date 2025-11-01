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

pub struct Change_Struct_Field_Type_38;

impl Mutator for Change_Struct_Field_Type_38 {
    fn name(&self) -> &str {
        "Change_Struct_Field_Type_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut available_types = Vec::new();

        for item in &file.items {
            if let Item::Struct(ItemStruct { fields, .. }) = item {
                for field in fields.iter() {
                    if let syn::Type::Path(TypePath { path, .. }) = &field.ty {
                        available_types.push(path.clone());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(ItemStruct { fields, .. }) = item {
                for field in fields.iter_mut() {
                    if let syn::Type::Path(TypePath { path, .. }) = &field.ty {
                        if let Some(new_type) = available_types.choose(&mut rng) {
                            if new_type != path {
                                field.ty = syn::Type::Path(TypePath { path: new_type.clone(), qself: None });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator 'Change_Struct_Field_Type_38' identifies struct definitions within the Rust code and selects a field to change its type. It chooses a new type from existing types within the program, ensuring the type change is feasible and consistent throughout the code. This mutation aims to explore potential type mismatches and trigger compiler bugs related to type checking."
    }
}