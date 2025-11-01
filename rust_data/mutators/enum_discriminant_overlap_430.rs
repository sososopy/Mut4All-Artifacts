use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Enum_Discriminant_Overlap_430;

impl Mutator for Enum_Discriminant_Overlap_430 {
    fn name(&self) -> &str {
        "Enum_Discriminant_Overlap_430"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut rng = thread_rng();
                let mut discriminant_values = Vec::new();

                for variant in &item_enum.variants {
                    if let Some((_, Expr::Lit(expr_lit))) = &variant.discriminant {
                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                            if let Ok(value) = lit_int.base10_parse::<i64>() {
                                discriminant_values.push((variant.ident.clone(), value));
                            }
                        }
                    }
                }

                if discriminant_values.len() > 1 {
                    if let Some((variant_ident, _)) = discriminant_values.choose(&mut rng) {
                        if let Some((_, Expr::Lit(expr_lit))) = &mut item_enum.variants.iter_mut()
                            .find(|v| v.ident == *variant_ident)
                            .and_then(|v| v.discriminant.as_mut()) {
                                if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                                    *lit_int = syn::LitInt::new(&discriminant_values[0].1.to_string(), Span::call_site());
                                }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with explicitly assigned discriminant values. It selects a variant and modifies its discriminant value to overlap with another variant's value, testing the compiler's handling of overlapping or non-sequential discriminant values."
    }
}