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

pub struct Mismatched_Generic_Types_In_Return_31;

impl Mutator for Mismatched_Generic_Types_In_Return_31 {
    fn name(&self) -> &str {
        "Mismatched_Generic_Types_In_Return_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(TypeParamBound::Trait(trait_bound)) = type_impl_trait.bounds.first() {
                            let mut trait_bound = trait_bound.clone();
                            if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                    if let Some(GenericArgument::Type(Type::Path(type_path))) = angle_args.args.first_mut() {
                                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                                            last_segment.ident = Ident::new("B", Span::call_site());
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
        "The mutation operator targets functions with `impl Trait` return types, specifically altering the generic type used in the return signature to a different type than used in the function body. This transformation aims to introduce type mismatches that challenge the compiler's type checking and generic resolution logic, potentially exposing bugs in these systems."
    }
}