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

pub struct Substitute_Constants_In_Enums_339;

impl Mutator for Substitute_Constants_In_Enums_339 {
    fn name(&self) -> &str {
        "Substitute_Constants_In_Enums_339"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    if let Some((_, expr)) = &mut variant.discriminant {
                        let new_expr = match expr {
                            Expr::Lit(lit) => {
                                let new_value = match &lit.lit {
                                    Lit::Int(int) => {
                                        let value: i32 = int.base10_parse().unwrap();
                                        let new_value = value + 1;
                                        parse_quote! { #new_value }
                                    }
                                    Lit::Float(float) => {
                                        let value: f64 = float.base10_parse().unwrap();
                                        let new_value = value + 1.0;
                                        parse_quote! { #new_value }
                                    }
                                    _ => unreachable!(),
                                };
                                new_value
                            }
                            _ => expr.clone(),
                        };
                        *expr = new_expr;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes constant expressions in enum variants with new constant expressions of the same type. This transformation tests the compiler's handling of constant evaluation in different contexts and may trigger bugs related to constant evaluation."
    }
}