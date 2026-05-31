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

pub struct Missing_Lifetime_In_Trait_Implementation_90;

impl Mutator for Missing_Lifetime_In_Trait_Implementation_90 {
    fn name(&self) -> &str {
        "Missing_Lifetime_In_Trait_Implementation_90"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        let trait_name = &last_segment.ident;
                        if let Some(trait_def) = find_trait_definition(file, trait_name) {
                            let has_lifetime = trait_def.generics.lifetimes().count() > 0;
                            if has_lifetime {
                                item_impl.generics.params.clear();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies trait implementations where lifetime parameters are required but omitted. It specifically targets async functions within traits, ensuring that the implementation of the trait for a struct does not specify the lifetime parameters, even though they are present in the trait definition. This mutation aims to test the Rust compiler's handling of lifetimes and async functions, potentially leading to internal compiler errors related to lifetime elision and region inference."
    }
}

fn find_trait_definition<'a>(file: &'a syn::File, trait_name: &Ident) -> Option<&'a ItemTrait> {
    for item in &file.items {
        if let Item::Trait(item_trait) = item {
            if &item_trait.ident == trait_name {
                return Some(item_trait);
            }
        }
    }
    None
}