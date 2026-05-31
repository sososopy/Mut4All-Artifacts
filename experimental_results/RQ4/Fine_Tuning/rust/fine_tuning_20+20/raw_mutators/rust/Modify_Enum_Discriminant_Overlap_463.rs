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

pub struct Modify_Enum_Discriminant_Overlap_463;

impl Mutator for Modify_Enum_Discriminant_Overlap_463 {
    fn name(&self) -> &str {
        "Modify_Enum_Discriminant_Overlap_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut discriminant_values = Vec::new();
                for variant in &item_enum.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        if let Expr::Lit(expr_lit) = expr {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if let Ok(value) = lit_int.base10_parse::<i64>() {
                                    discriminant_values.push(value);
                                }
                            }
                        }
                    }
                }
                if discriminant_values.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let overlap_value = *discriminant_values.choose(&mut rng).unwrap();
                let mut modified = false;
                for variant in &mut item_enum.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        if let Expr::Lit(expr_lit) = expr {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if let Ok(value) = lit_int.base10_parse::<i64>() {
                                    if value != overlap_value {
                                        let new_expr: Expr = parse_quote!(#overlap_value);
                                        variant.discriminant = Some((Default::default(), new_expr));
                                        modified = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if modified {
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets enums with explicit discriminant values, modifying one or more variants to have overlapping or non-sequential discriminants. By introducing conflicts in discriminant values, it tests the compiler's ability to handle enum variant resolution and discriminant interpretation, potentially exposing weaknesses in how the compiler manages enum discriminants during code generation and type checking."
    }
}