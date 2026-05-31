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

pub struct Incorrect_Trait_Bound_Application_30;

impl Mutator for Incorrect_Trait_Bound_Application_30 {
    fn name(&self) -> &str {
        "Incorrect_Trait_Bound_Application_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        if let Some(bounds) = &mut type_param.bounds.first_mut() {
                            if let syn::TypeParamBound::Trait(trait_bound) = bounds {
                                let new_generic_ident = Ident::new("U", Span::call_site());
                                trait_bound.path.segments.push(syn::PathSegment {
                                    ident: new_generic_ident,
                                    arguments: syn::PathArguments::None,
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with generic parameters constrained by a trait bound. It modifies the trait bound by appending an undefined generic type `U` to the trait path. This syntactically valid but semantically incorrect transformation aims to test the compiler's trait system handling, potentially uncovering bugs related to trait bound evaluation and type checking."
    }
}