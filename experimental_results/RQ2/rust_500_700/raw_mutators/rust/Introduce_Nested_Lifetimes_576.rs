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

pub struct Introduce_Nested_Lifetimes_576;

impl Mutator for Introduce_Nested_Lifetimes_576 {
    fn name(&self) -> &str {
        "Introduce_Nested_Lifetimes_576"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut has_outer_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(lifetimes) = &trait_bound.lifetimes {
                                    if lifetimes.lifetimes.iter().any(|lt| lt.lifetime.ident == "a") {
                                        has_outer_lifetime = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if has_outer_lifetime {
                            item_fn.sig.generics.params.push(parse_quote!('b: 'a));
                            for bound in &mut type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                        if lifetimes.lifetimes.iter().any(|lt| lt.lifetime.ident == "a") {
                                            lifetimes.lifetimes.push(parse_quote!('b));
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