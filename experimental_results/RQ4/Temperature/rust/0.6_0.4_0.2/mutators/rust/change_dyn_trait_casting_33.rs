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

pub struct Change_Dyn_Trait_Casting_33;

impl Mutator for Change_Dyn_Trait_Casting_33 {
    fn name(&self) -> &str {
        "Change_Dyn_Trait_Casting_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::TraitObject(trait_object) = &mut **ty {
                        let mut rng = thread_rng();
                        let traits = vec!["Clone", "PartialEq", "Eq", "Ord", "PartialOrd"];
                        let new_trait = traits.choose(&mut rng).unwrap();
                        trait_object.bounds.clear();
                        trait_object.bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new(new_trait, Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with return types involving `dyn*` trait objects. It replaces the current trait with a randomly selected trait from a predefined list (e.g., Clone, PartialEq, etc.). This transformation explores the compiler's robustness in handling dynamic trait objects with varying trait bounds, potentially exposing internal errors related to trait handling."
    }
}