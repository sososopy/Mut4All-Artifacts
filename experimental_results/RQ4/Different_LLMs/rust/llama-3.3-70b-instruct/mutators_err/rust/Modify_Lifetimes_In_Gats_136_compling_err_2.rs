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

pub struct Modify_Lifetimes_In_Gats_136;

impl Mutator for Modify_Lifetimes_In_Gats_136 {
    fn name(&self) -> &str {
        "Modify_Lifetimes_In_Gats_136"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for assoc_type in &mut trait_item.generics.params {
                    if let GenericParam::Type(type_param) = assoc_type {
                        if let Some(lifetime_params) = &type_param.bounds {
                            for bound in lifetime_params {
                                if let TypeParamBound::Lifetime(lifetime) = bound {
                                    let new_lifetime = if lifetime.ident == "a" {
                                        Lifetime::new("'static", Span::call_site())
                                    } else {
                                        Lifetime::new("'_", Span::call_site())
                                    };
                                    *bound = TypeParamBound::Lifetime(new_lifetime);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic associated types (GATs) in traits and modifies the lifetimes of the associated types. It changes the lifetime of the GAT to a different lifetime, such as changing 'a to 'static or '_."
    }
}