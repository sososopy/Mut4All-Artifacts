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

pub struct Modify_Enum_Variant_Type_306;

impl Mutator for Modify_Enum_Variant_Type_306 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Type_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut variant_types: HashSet<Type> = HashSet::new();
                for variant in &item_enum.variants {
                    if let syn::Fields::Unnamed(fields) = &variant.fields {
                        if let Some(field) = fields.unnamed.first() {
                            variant_types.insert(field.ty.clone());
                        }
                    }
                }

                if variant_types.len() > 1 {
                    let mut rng = thread_rng();
                    let types_vec: Vec<Type> = variant_types.into_iter().collect();
                    let new_type = types_vec.choose(&mut rng).unwrap();

                    for variant in &mut item_enum.variants {
                        if let syn::Fields::Unnamed(fields) = &mut variant.fields {
                            if let Some(field) = fields.unnamed.first_mut() {
                                field.ty = new_type.clone();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with multiple variants, each having different associated types. It randomly selects a type from the existing variants and applies it to all variants, potentially causing type mismatches in the code using these variants. This transformation aims to test the compiler's handling of type resolution and variant management under altered type expectations."
    }
}