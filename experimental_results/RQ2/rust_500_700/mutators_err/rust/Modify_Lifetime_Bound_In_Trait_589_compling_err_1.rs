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

pub struct Modify_Lifetime_Bound_In_Trait_589;

impl Mutator for Modify_Lifetime_Bound_In_Trait_589 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Bound_In_Trait_589"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(ref mut trait_item) = item {
                let mut has_static_lifetime = false;
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(ref mut type_item) = item {
                        if let Some(bounds) = &mut type_item.bounds {
                            for bound in bounds.iter_mut() {
                                if let TypeParamBound::Lifetime(lifetime) = bound {
                                    if lifetime.ident == "static" {
                                        has_static_lifetime = true;
                                        *lifetime = Lifetime::new("'a", Span::call_site());
                                    }
                                }
                            }
                        }
                    }
                }
                if has_static_lifetime {
                    trait_item.generics.params.push(GenericParam::Lifetime(LifetimeDef {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'a", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}