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

pub struct Modify_Trait_Method_Self_Type_With_Additional_Traits_304;

impl Mutator for Modify_Trait_Method_Self_Type_With_Additional_Traits_304 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Self_Type_With_Additional_Traits_304"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                existing_traits.insert(item_trait.ident.to_string());
            }
        }
        let mut has_trait3 = false;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "Trait3" {
                    has_trait3 = true;
                }
            }
        }
        let mut target_trait_name = None;
        let mut target_method_name = None;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        if item_fn.sig.asyncness.is_some() {
                            for arg in &item_fn.sig.inputs {
                                if let FnArg::Typed(pat_type) = arg {
                                    if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                                        let mut trait_count = 0;
                                        for bound in &type_impl_trait.bounds {
                                            if let TypeParamBound::Trait(_) = bound {
                                                trait_count += 1;
                                            }
                                        }
                                        if trait_count >= 2 {
                                            target_trait_name = Some(item_trait.ident.clone());
                                            target_method_name = Some(item_fn.sig.ident.clone());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        let mut replacement_trait = None;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "Trait3" {
                    replacement_trait = Some(item_trait.ident.clone());
                }
            }
        }
        if replacement_trait.is_none() {
            for item in &file.items {
                if let Item::Trait(item_trait) = item {
                    if item_trait.ident != *target_trait_name.as_ref().unwrap() {
                        replacement_trait = Some(item_trait.ident.clone());
                    }
                }
            }
        }
        if replacement_trait.is_none() {
            replacement_trait = Some(Ident::new("Trait3", Span::call_site()));
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == *target_trait_name.as_ref().unwrap() {
                    for item in &mut item_trait.items {
                        if let TraitItem::Fn(item_fn) = item {
                            if item_fn.sig.ident == *target_method_name.as_ref().unwrap() {
                                for arg in &mut item_fn.sig.inputs {
                                    if let FnArg::Typed(pat_type) = arg {
                                        if let Type::ImplTrait(type_impl_trait) =
                                            &mut *pat_type.ty
                                        {
                                            type_impl_trait.bounds.push(
                                                TypeParamBound::Trait(TraitBound {
                                                    paren_token: None,
                                                    modifier: TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: SynPath::from(
                                                        replacement_trait.as_ref().unwrap().clone(),
                                                    ),
                                                }),
                                            );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_trait3 {
            let trait_item: Item = parse_quote! {
                pub trait Trait3 {}
            };
            file.items.push(trait_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify an async trait method using self with multiple trait bounds and add an additional trait constraint, either reusing an existing trait or introducing a new simple trait, to enhance trait-bound complexity and stress the compiler's trait resolution capabilities."
    }
}