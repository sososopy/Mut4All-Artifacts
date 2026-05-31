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

pub struct Enum_Variant_Overloading_73;

impl Mutator for Enum_Variant_Overloading_73 {
    fn name(&self) -> &str {
        "Enum_Variant_Overloading_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let mut variant_names: HashSet<String> = HashSet::new();
                let mut repeated_variant_indices: Vec<(usize, String)> = Vec::new();

                for (index, variant) in item_enum.variants.iter().enumerate() {
                    let variant_name = variant.ident.to_string();
                    if variant_names.contains(&variant_name) {
                        repeated_variant_indices.push((index, variant_name));
                    } else {
                        variant_names.insert(variant_name);
                    }
                }

                for (index, variant_name) in repeated_variant_indices {
                    let new_variant_name = format!("{}{}", variant_name, index);
                    item_enum.variants[index].ident = Ident::new(&new_variant_name, Span::call_site());
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with overloaded variant names, which could potentially lead to type confusion or incorrect assumptions by the compiler. By ensuring that each variant name is unique, we reduce ambiguity and potential for misinterpretation by the compiler, which can help in identifying areas where the compiler is not robust in handling overloading scenarios."
    }
}