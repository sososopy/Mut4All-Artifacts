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

pub struct Modify_Lifetime_Annotations_In_Trait_Methods_191;

impl Mutator for Modify_Lifetime_Annotations_In_Trait_Methods_191 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_In_Trait_Methods_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Fn(method) = item {
                        let mut lifetimes = vec![];
                        for param in &method.sig.generics.params {
                            if let GenericParam::Lifetime(lifetime) = param {
                                lifetimes.push(lifetime.clone());
                            }
                        }
                        if lifetimes.len() >= 2 {
                            let first_lifetime = &lifetimes[0];
                            let second_lifetime = &lifetimes[1];
                            method.sig.generics.params.clear();
                            method.sig.generics.params.push(GenericParam::Lifetime(first_lifetime.clone()));
                            method.sig.generics.params.push(GenericParam::Lifetime(second_lifetime.clone()));
                            if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                if let Type::ImplTrait(type_impl_trait) = &mut **ty {
                                    for bound in &mut type_impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                                lifetimes.lifetimes.clear();
                                                lifetimes.lifetimes.push(first_lifetime.clone());
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
        ""
    }
}