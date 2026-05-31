use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
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

pub struct Replace_Enum_Variant_465;

impl Mutator for Replace_Enum_Variant_465 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_465"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let variants: Vec<_> = enum_item.variants.clone();
                if variants.len() > 1 {
                    let mut rng = thread_rng();
                    let index_to_replace = rng.gen_range(0..variants.len());
                    let replacement_index = rng.gen_range(0..variants.len());
                    while replacement_index == index_to_replace {
                        let replacement_index = rng.gen_range(0..variants.len());
                    }
                    let replacement_variant = &variants[replacement_index];
                    enum_item.variants[index_to_replace].ident = replacement_variant.ident.clone();
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions and replaces one of the variants with another existing variant from the same enum. This transformation can potentially trigger bugs related to transmutability, especially when the replaced variant has a different size or layout than the original one."
    }
}