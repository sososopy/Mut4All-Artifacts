use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Simplify_Static_Type_395;

impl Mutator for Simplify_Static_Type_395 {
    fn name(&self) -> &str {
        "Simplify_Static_Type_395"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::TraitObject(type_trait_object) = &mut *item_static.ty {
                    if type_trait_object.bounds.len() > 1 {
                        if let Some(first_bound) = type_trait_object.bounds.first().cloned() {
                            type_trait_object.bounds = Punctuated::new();
                            type_trait_object.bounds.push(first_bound);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets static variable declarations with complex unsized types. It simplifies these types by reducing multiple trait bounds to a single trait bound. This is done by checking if the static variable's type is a TraitObject with more than one bound. If so, it retains only the first bound, effectively simplifying the type. This mutation is designed to test the Rust compiler's handling of unsized types, especially in the context of static variables."
    }
}