use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Inconsistent_Associated_Type_Usage_359;

impl Mutator for Inconsistent_Associated_Type_Usage_359 {
    fn name(&self) -> &str {
        "Inconsistent_Associated_Type_Usage_359"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls: Vec<(usize, usize)> = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    let trait_path = item_impl.trait_.as_ref().unwrap().1.clone();
                    let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                    trait_impls.push((i, trait_ident.span().start().line));
                }
            }
        }
        if trait_impls.is_empty() {
            return;
        }
        trait_impls.sort_by_key(|k| k.1);
        let mut rng = thread_rng();
        let chosen_impl_index = rng.gen_range(0..trait_impls.len());
        let chosen_impl = trait_impls[chosen_impl_index];
        let mut trait_def_indices: Vec<usize> = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Trait(_) = item {
                trait_def_indices.push(i);
            }
        }
        let mut chosen_trait_index: Option<usize> = None;
        for &index in &trait_def_indices {
            if index < chosen_impl.0 {
                chosen_trait_index = Some(index);
            }
        }
        if chosen_trait_index.is_none() {
            return;
        }
        let chosen_trait_index = chosen_trait_index.unwrap();
        let chosen_trait = file.items[chosen_trait_index].clone();
        let chosen_impl = file.items[chosen_impl.0].clone();
        let mut associated_types: Vec<Ident> = Vec::new();
        if let Item::Trait(trait_item) = chosen_trait {
            for item in &trait_item.items {
                if let TraitItem::Type(type_item) = item {
                    associated_types.push(type_item.ident.clone());
                }
            }
        }
        if associated_types.is_empty() {
            return;
        }
        let chosen_associated_type = associated_types.choose(&mut rng).unwrap();
        if let Item::Impl(mut item_impl) = chosen_impl {
            let mut found = false;
            for impl_item in &mut item_impl.items {
                if let ImplItem::Type(type_item) = impl_item {
                    if type_item.ident == *chosen_associated_type {
                        type_item.ty = parse_quote!([u8; 4]);
                        found = true;
                        break;
                    }
                }
            }
            if found {
                file.items[chosen_impl.0] = Item::Impl(item_impl);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets trait implementations by first identifying all trait impls and their corresponding trait definitions. It randomly selects a trait impl and its associated trait, then modifies an associated type within the impl to use an incompatible type context, such as replacing a scalar type with an array. This transformation aims to introduce logical inconsistencies in the use of associated types, challenging the compiler's ability to enforce type well-formedness and trait coherence."
    }
}