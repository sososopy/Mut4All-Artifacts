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

pub struct Omit_Trait_Method_Implementation_326;

impl Mutator for Omit_Trait_Method_Implementation_326 {
    fn name(&self) -> &str {
        "Omit_Trait_Method_Implementation_326"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_method_map = HashMap::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let trait_ident = item_trait.ident.clone();
                let method_idents: Vec<Ident> = item_trait
                    .items
                    .iter()
                    .filter_map(|trait_item| {
                        if let TraitItem::Fn(trait_fn) = trait_item {
                            Some(trait_fn.sig.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                trait_method_map.insert(trait_ident, method_idents);
            }
        }
        let mut rng = thread_rng();
        let mut impl_trait_map: HashMap<Ident, Vec<Ident>> = HashMap::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(trait_ident) = path.segments.last().map(|seg| seg.ident.clone()) {
                        if let Some(method_idents) = trait_method_map.get(&trait_ident) {
                            let implemented_methods: Vec<Ident> = item_impl
                                .items
                                .iter()
                                .filter_map(|impl_item| {
                                    if let ImplItem::Fn(impl_fn) = impl_item {
                                        Some(impl_fn.sig.ident.clone())
                                    } else {
                                        None
                                    }
                                })
                                .collect();
                            let missing_methods: Vec<Ident> = method_idents
                                .iter()
                                .filter(|ident| !implemented_methods.contains(ident))
                                .cloned()
                                .collect();
                            if !missing_methods.is_empty() {
                                impl_trait_map.insert(trait_ident.clone(), missing_methods);
                            }
                        }
                    }
                }
            }
        }
        let mut candidates = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(trait_ident) = path.segments.last().map(|seg| seg.ident.clone()) {
                        if let Some(method_idents) = impl_trait_map.get(&trait_ident) {
                            candidates.push((trait_ident.clone(), method_idents.clone()));
                        }
                    }
                }
            }
        }
        candidates.shuffle(&mut rng);
        if let Some((target_trait, target_methods)) = candidates.first() {
            for item in &mut file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(trait_ident) = path.segments.last().map(|seg| seg.ident.clone())
                        {
                            if &trait_ident == target_trait {
                                item_impl.items.retain(|impl_item| {
                                    if let ImplItem::Fn(impl_fn) = impl_item {
                                        !target_methods.contains(&impl_fn.sig.ident)
                                    } else {
                                        true
                                    }
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that are missing one or more required methods. It first identifies all traits and their methods, then finds implementations that omit some of these methods. By selecting one such incomplete implementation, it removes any already-implemented methods from the impl block, ensuring the trait is not fully implemented. This transformation is designed to trigger compiler errors related to incomplete trait implementations, potentially exposing bugs in how the compiler handles trait method resolution and error reporting for missing methods."
    }
}