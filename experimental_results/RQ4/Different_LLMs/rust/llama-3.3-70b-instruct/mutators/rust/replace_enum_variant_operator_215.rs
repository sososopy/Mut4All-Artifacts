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

pub struct Replace_Enum_Variant_Operator_215;

impl Mutator for Replace_Enum_Variant_Operator_215 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Operator_215"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants: Punctuated<syn::Variant, Comma> = enum_item.variants.clone();
                if !variants.is_empty() {
                    let mut rng = thread_rng();
                    let index_to_replace = rng.gen_range(0..variants.len());
                    let new_variant_name = Ident::new(
                        &format!("NewVariant_{}", rng.gen_range(0..100)),
                        Span::call_site(),
                    );
                    let new_variant = syn::Variant {
                        attrs: vec![],
                        ident: new_variant_name,
                        fields: syn::Fields::Unit,
                        discriminant: None,
                    };
                    variants[index_to_replace] = new_variant;
                    enum_item.variants = variants;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an existing variant in an enum definition with a new one. This transformation tests the compiler's handling of enum definitions and their variants, potentially leading to issues with pattern matching, variant access, or enum value manipulation."
    }
}