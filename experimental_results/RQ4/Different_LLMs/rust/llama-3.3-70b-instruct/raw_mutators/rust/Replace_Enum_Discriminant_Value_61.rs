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

pub struct Replace_Enum_Discriminant_Value_61;

impl Mutator for Replace_Enum_Discriminant_Value_61 {
    fn name(&self) -> &str {
        "Replace_Enum_Discriminant_Value_61"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    if let Some((_, expr)) = &mut variant.discriminant {
                        let new_value = thread_rng().gen_range(0..100);
                        *expr = parse_quote! { #new_value };
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the discriminant value of an enum variant with a random value. This transformation aims to test the compiler's ability to handle invalid or out-of-range discriminant values and may lead to ICEs or errors in constant evaluation."
    }
}