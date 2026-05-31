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

pub struct Enum_Variant_Overloading_73;

impl Mutator for Enum_Variant_Overloading_73 {
    fn name(&self) -> &str {
        "Enum_Variant_Overloading_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut variant_names = HashSet::new();
                let mut duplicates = Vec::new();

                for variant in &item_enum.variants {
                    let variant_name = variant.ident.to_string();
                    if !variant_names.insert(variant_name.clone()) {
                        duplicates.push(variant_name);
                    }
                }

                for duplicate in duplicates {
                    let mut counter = 1;
                    for variant in &mut item_enum.variants {
                        if variant.ident == duplicate {
                            variant.ident = Ident::new(
                                &format!("{}{}", duplicate, counter),
                                Span::call_site(),
                            );
                            counter += 1;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies enums with overloaded variant names, ensuring each variant name is unique by appending a counter to duplicates. This transformation reduces ambiguity and potential misinterpretation by the compiler, testing its robustness in handling overloaded variant scenarios."
    }
}