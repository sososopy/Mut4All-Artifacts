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

pub struct Alter_Lifetime_Annotations_40;

impl Mutator for Alter_Lifetime_Annotations_40 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                
                let lifetimes: Vec<&LifetimeParam> = func.sig.generics.params.iter().filter_map(|param| {
                    if let GenericParam::Lifetime(lifetime) = param {
                        Some(lifetime)
                    } else {
                        None
                    }
                }).collect();
                
                if !lifetimes.is_empty() {
                    // Randomly decide to remove or modify a lifetime
                    if rng.gen_bool(0.5) {
                        // Remove a lifetime
                        let index_to_remove = rng.gen_range(0..lifetimes.len());
                        func.sig.generics.params = func.sig.generics.params.iter().filter(|param| {
                            if let GenericParam::Lifetime(lifetime) = param {
                                lifetime != lifetimes[index_to_remove]
                            } else {
                                true
                            }
                        }).cloned().collect();
                    } else {
                        // Modify a lifetime relationship
                        let new_lifetime: Lifetime = parse_quote!('static);
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                    type_ref.lifetime = Some(new_lifetime.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with lifetimes. It randomly decides to either remove a lifetime parameter to test the compiler's inference capabilities or replace an existing lifetime with 'static to challenge lifetime resolution. This aims to explore how the Rust compiler handles complex lifetime relationships and potential ICEs."
    }
}