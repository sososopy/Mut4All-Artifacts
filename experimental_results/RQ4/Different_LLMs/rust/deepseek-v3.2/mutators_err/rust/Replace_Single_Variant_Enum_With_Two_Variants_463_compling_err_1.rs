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

pub struct Replace_Single_Variant_Enum_With_Two_Variants_463;

impl Mutator for Replace_Single_Variant_Enum_With_Two_Variants_463 {
    fn name(&self) -> &str {
        "Replace_Single_Variant_Enum_With_Two_Variants_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum.variants.len() == 1 {
                    let original_variant = &item_enum.variants[0];
                    let original_name = &original_variant.ident;
                    let new_variant_name = if original_name.to_string().len() == 1 && original_name.to_string().chars().next().unwrap().is_uppercase() {
                        Ident::new("W", original_name.span())
                    } else {
                        Ident::new(&format!("{}Second", original_name), original_name.span())
                    };
                    let new_variant = Variant {
                        attrs: Vec::new(),
                        ident: new_variant_name,
                        fields: Fields::Unit,
                        discriminant: None,
                    };
                    item_enum.variants.push(new_variant);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions with exactly one variant. It adds a second unit variant, preserving the original variant's structure and any representation attributes. This transformation can trigger discriminant calculation errors in transmutability analysis, especially with zero-sized types or unusual enum layouts, potentially reproducing ICEs related to enum variant discriminants."
    }
}