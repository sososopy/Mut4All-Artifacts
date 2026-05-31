use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Enum_Variant_Type_463;

impl Mutator for Replace_Enum_Variant_Type_463 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Type_463"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut types: Vec<Type> = Vec::new();
                for variant in &enum_item.variants {
                    match &variant.fields {
                        Fields::Named(fields) => {
                            for field in &fields.named {
                                types.push(field.ty.clone());
                            }
                        }
                        Fields::Unnamed(fields) => {
                            for field in &fields.unnamed {
                                types.push(field.ty.clone());
                            }
                        }
                        Fields::Unit => {}
                    }
                }
                for variant in &mut enum_item.variants {
                    match &mut variant.fields {
                        Fields::Named(fields) => {
                            for field in &mut fields.named {
                                if !types.is_empty() {
                                    let new_ty = types.choose(&mut thread_rng()).unwrap().clone();
                                    field.ty = new_ty;
                                }
                            }
                        }
                        Fields::Unnamed(fields) => {
                            for field in &mut fields.unnamed {
                                if !types.is_empty() {
                                    let new_ty = types.choose(&mut thread_rng()).unwrap().clone();
                                    field.ty = new_ty;
                                }
                            }
                        }
                        Fields::Unit => {}
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the type of an enum variant with another type that already exists in the code. This transformation tests the compiler's ability to handle changed types of enum variants and may lead to bugs related to transmutability."
    }
}