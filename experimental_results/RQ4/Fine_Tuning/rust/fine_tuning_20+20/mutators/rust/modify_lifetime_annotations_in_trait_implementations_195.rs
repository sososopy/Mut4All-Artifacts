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

pub struct Modify_Lifetime_Annotations_In_Trait_Implementations_195;

impl Mutator for Modify_Lifetime_Annotations_In_Trait_Implementations_195 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_In_Trait_Implementations_195"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_items: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Trait(trait_item) => Some(trait_item.clone()),
                _ => None,
            })
            .collect();
        let impl_items: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Impl(item_impl) => Some(item_impl.clone()),
                _ => None,
            })
            .collect();
        for trait_item in trait_items {
            let trait_name = trait_item.ident.to_string();
            let trait_lifetimes: Vec<_> = trait_item
                .generics
                .params
                .iter()
                .filter_map(|param| match param {
                    GenericParam::Lifetime(lifetime) => Some(lifetime.lifetime.clone()),
                    _ => None,
                })
                .collect();
            let mut new_file = file.clone();
            for item in &mut new_file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(ident) = path.get_ident() {
                            if ident == &trait_item.ident {
                                let impl_lifetimes: Vec<_> = item_impl
                                    .generics
                                    .params
                                    .iter()
                                    .filter_map(|param| match param {
                                        GenericParam::Lifetime(lifetime) => {
                                            Some(lifetime.lifetime.clone())
                                        }
                                        _ => None,
                                    })
                                    .collect();
                                if trait_lifetimes.len() == impl_lifetimes.len() {
                                    let new_lifetimes: Vec<_> = (0..impl_lifetimes.len())
                                        .map(|i| Lifetime::new(&format!("'c{}", i), Span::call_site()))
                                        .collect();
                                    for (old, new) in impl_lifetimes.iter().zip(&new_lifetimes) {
                                        let mut new_file = file.clone();
                                        for item in &mut new_file.items {
                                            let mut replacer = LifetimeReplacer {
                                                target: old,
                                                replacement: new,
                                            };
                                            replacer.visit_item_mut(item);
                                        }
                                        *file = new_file;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            *file = new_file;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify traits with lifetime parameters and their corresponding implementations. Replace the lifetimes in the implementation with new, non-universal lifetimes to induce lifetime mismatches, testing the compiler's handling of lifetime resolution and trait implementation validity."
    }
}

struct LifetimeReplacer<'a> {
    target: &'a Lifetime,
    replacement: &'a Lifetime,
}

impl<'a> VisitMut for LifetimeReplacer<'a> {
    fn visit_lifetime_mut(&mut self, lifetime: &mut Lifetime) {
        if lifetime == self.target {
            *lifetime = self.replacement.clone();
        }
    }
}