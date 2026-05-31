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

pub struct Add_Redundant_Lifetime_Params_34;

impl Mutator for Add_Redundant_Lifetime_Params_34 {
    fn name(&self) -> &str {
        "Add_Redundant_Lifetime_Params_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    let generics = &mut item_fn.sig.generics;
                    let has_lifetime = generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                    if has_lifetime {
                        let new_lifetime: LifetimeParam = parse_quote!('unused_lt);
                        generics.params.push(GenericParam::Lifetime(new_lifetime));
                    }
                }
                Item::Trait(item_trait) => {
                    let generics = &mut item_trait.generics;
                    let has_lifetime = generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                    if has_lifetime {
                        let new_lifetime: LifetimeParam = parse_quote!('unused_lt);
                        generics.params.push(GenericParam::Lifetime(new_lifetime));
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}