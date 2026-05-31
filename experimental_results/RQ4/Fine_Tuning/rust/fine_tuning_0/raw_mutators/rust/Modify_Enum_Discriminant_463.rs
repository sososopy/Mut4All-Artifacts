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

pub struct Modify_Enum_Discriminant_463;

impl Mutator for Modify_Enum_Discriminant_463 {
    fn name(&self) -> &str {
        "Modify_Enum_Discriminant_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut rng = thread_rng();
                let mut discriminant_values: Vec<i64> = Vec::new();
                
                for variant in &item_enum.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        if let syn::Expr::Lit(syn::ExprLit {
                            lit: syn::Lit::Int(lit_int),
                            ..
                        }) = expr
                        {
                            if let Ok(value) = lit_int.base10_parse::<i64>() {
                                discriminant_values.push(value);
                            }
                        }
                    }
                }
                
                if discriminant_values.len() > 1 {
                    let index_to_modify = rng.gen_range(0..discriminant_values.len());
                    let new_value = discriminant_values.choose(&mut rng).unwrap_or(&0);
                    
                    if let Some(variant) = item_enum.variants.iter_mut().nth(index_to_modify) {
                        if let Some((_, expr)) = &mut variant.discriminant {
                            *expr = syn::Expr::Lit(syn::ExprLit {
                                lit: syn::Lit::Int(syn::LitInt::new(
                                    &new_value.to_string(),
                                    Span::call_site(),
                                )),
                                attrs: Vec::new(),
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies enums with explicitly assigned discriminant values and modifies one variant's discriminant to overlap with another. This tests the compiler's handling of overlapping or non-sequential discriminant values, potentially leading to misinterpretation or incorrect handling of such enums during compilation."
    }
}