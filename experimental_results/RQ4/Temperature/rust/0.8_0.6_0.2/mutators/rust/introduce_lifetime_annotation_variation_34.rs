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

pub struct Introduce_Lifetime_Annotation_Variation_34;

impl Mutator for Introduce_Lifetime_Annotation_Variation_34 {
    fn name(&self) -> &str {
        "Introduce_Lifetime_Annotation_Variation_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = trait_item {
                        let generics = &mut type_item.generics;
                        // Modify the number of lifetime parameters or change the annotations
                        if generics.params.is_empty() {
                            generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: Lifetime::new("'a", Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                        } else {
                            // Introduce variations
                            if thread_rng().gen_bool(0.5) {
                                // Remove a lifetime parameter
                                generics.params.clear();
                            } else {
                                // Add an extra lifetime parameter
                                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: Lifetime::new("'b", Span::call_site()),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));
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