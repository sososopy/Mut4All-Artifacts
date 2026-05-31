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
    ItemTrait, TraitItem,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Function_Signature_With_Lifetime_Annotations_250;

impl Mutator for Modify_Trait_Function_Signature_With_Lifetime_Annotations_250 {
    fn name(&self) -> &str {
        "Modify_Trait_Function_Signature_With_Lifetime_Annotations_250"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_fn in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_fn {
                        if let Some(ref mut generics) = method.sig.generics.params.iter_mut().find_map(|param| {
                            if let syn::GenericParam::Lifetime(lifetime_param) = param {
                                Some(lifetime_param)
                            } else {
                                None
                            }
                        }) {
                            generics.bounds.push(syn::Lifetime::new(&generics.lifetime.ident.to_string(), Span::call_site()));
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