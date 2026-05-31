use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Remove_Lifetime_Parameter_167;

impl Mutator for Remove_Lifetime_Parameter_167 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameter_167"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if let Some(lifetimes) = &trait_item.generics.lifetimes {
                    let mut new_lifetimes = BoundLifetimes {
                        for_token: lifetimes.for_token,
                        lifetimes: Default::default(),
                        gt_token: lifetimes.gt_token,
                        lt_token: lifetimes.lt_token,
                    };
                    for lifetime in &lifetimes.lifetimes {
                        if let Some(lifetime_param) = &lifetime.lifetime {
                            new_lifetimes.lifetimes.push(Lifetime {
                                apostrophe: lifetime_param.apostrophe,
                                ident: lifetime_param.ident.clone(),
                            });
                        }
                    }
                    trait_item.generics.lifetimes = Some(new_lifetimes);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes a lifetime parameter from a trait definition, applying to code constructs that define traits with associated types and lifetime parameters. This transformation helps to expose bugs related to lifetime parameters and associated types in trait definitions."
    }
}