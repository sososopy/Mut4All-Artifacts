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

pub struct Replace_Trait_Bounds_271;

impl Mutator for Replace_Trait_Bounds_271 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_271"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_bounds = Vec::new();

        // Collect existing trait bounds
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for bound in &trait_item.supertraits {
                    trait_bounds.push(bound.clone());
                }
            }
            if let Item::Fn(func) = item {
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &type_path.path.segments.last().unwrap().arguments {
                                for arg in &angle_bracketed.args {
                                    if let GenericArgument::Type(type_arg) = arg {
                                        if let Type::TraitObject(trait_object) = type_arg {
                                            for bound in &trait_object.bounds {
                                                trait_bounds.push(bound.clone());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Replace existing trait bounds with new ones
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for bound in &mut trait_item.supertraits {
                    if let Some(new_bound) = trait_bounds.choose(&mut thread_rng()) {
                        *bound = new_bound.clone();
                    }
                }
            }
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut type_path.path.segments.last().unwrap().arguments {
                                for arg in &mut angle_bracketed.args {
                                    if let GenericArgument::Type(type_arg) = arg {
                                        if let Type::TraitObject(trait_object) = type_arg {
                                            for bound in &mut trait_object.bounds {
                                                if let Some(new_bound) = trait_bounds.choose(&mut thread_rng()) {
                                                    *bound = new_bound.clone();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The ReplaceTraitBounds mutation operator targets trait bounds in Rust code. It replaces existing trait bounds with new ones, potentially introducing bugs related to trait systems. This operator applies to trait definitions, function signatures, and type annotations."
    }
}