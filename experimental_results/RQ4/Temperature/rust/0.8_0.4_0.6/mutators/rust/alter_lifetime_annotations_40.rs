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
                if let Some(ref mut bound_lifetimes) = func.sig.generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Lifetime(lifetime_param) = param {
                        Some(lifetime_param)
                    } else {
                        None
                    }
                }) {
                    // Randomly decide to remove or replace lifetime
                    let mut rng = thread_rng();
                    if rng.gen_bool(0.5) {
                        // Remove lifetime
                        bound_lifetimes.lifetime = Lifetime::new("'_", Span::call_site());
                    } else {
                        // Replace lifetime
                        let new_lifetime = Lifetime::new("'b", Span::call_site());
                        bound_lifetimes.lifetime = new_lifetime;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator alters lifetime annotations in function signatures, either by removing them or replacing them with a new lifetime. This transformation can create mismatches in expected lifetime bounds, potentially exposing bugs in the compiler's handling of lifetime constraints, especially in complex scenarios involving higher-ranked trait bounds or associated types."
    }
}