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

pub struct Replace_Bound_Trait_53;

impl Mutator for Replace_Bound_Trait_53 {
    fn name(&self) -> &str {
        "Replace_Bound_Trait_53"
    }

    fn mutate(&self, file: &mut syn::File) {
        let alternative_traits = vec!["Copy", "Clone", "Debug", "Default"];
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::None = struct_item.generics {
                    continue;
                }
                let generics = &mut struct_item.generics;
                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait = alternative_traits.choose(&mut thread_rng()).unwrap();
                                trait_bound.path.segments[0].ident = Ident::new(new_trait, Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the existing trait bounds of a generic type with alternative trait bounds. This transformation aims to test the compiler's handling of trait bounds and their implications on const evaluation, potentially exposing bugs related to trait implementation and bounds checking."
    }
}