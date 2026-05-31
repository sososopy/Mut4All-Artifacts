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

pub struct Add_Const_Trait_Specialization_456;

impl Mutator for Add_Const_Trait_Specialization_456 {
    fn name(&self) -> &str {
        "Add_Const_Trait_Specialization_456"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut insertions = Vec::new();
        
        // First pass: collect all traits that need modification
        for (trait_idx, item) in file.items.iter().enumerate() {
            if let syn::Item::Trait(trait_item) = item {
                if !has_const_trait_attr(trait_item) {
                    let has_impl = file.items.iter().any(|other_item| {
                        if let syn::Item::Impl(impl_item) = other_item {
                            if let Some(poly_trait_ref) = &impl_item.trait_ {
                                if let Some(segment) = poly_trait_ref.1.segments.last() {
                                    segment.ident == trait_item.ident
                                } else {
                                    false
                                }
                            } else {
                                false
                            }
                        } else {
                            false
                        }
                    });
                    if has_impl {
                        let first_impl_index = file.items.iter().position(|other_item| {
                            if let syn::Item::Impl(impl_item) = other_item {
                                if let Some(poly_trait_ref) = &impl_item.trait_ {
                                    if let Some(segment) = poly_trait_ref.1.segments.last() {
                                        segment.ident == trait_item.ident
                                    } else {
                                        false
                                    }
                                } else {
                                    false
                                }
                            } else {
                                false
                            }
                        });
                        if let Some(index) = first_impl_index {
                            insertions.push((trait_idx, index));
                        }
                    }
                }
            }
        }
        
        // Second pass: perform insertions in reverse order to avoid index shifting
        for &(trait_idx, index) in insertions.iter().rev() {
            if let Some(item) = file.items.get(trait_idx) {
                if let syn::Item::Trait(trait_item) = item {
                    let trait_name = &trait_item.ident;
                    let new_impl = parse_quote! {
                        impl<T> const #trait_name for T where T: const Specialize { }
                    };
                    file.items.insert(index + 1, syn::Item::Impl(new_impl));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn has_const_trait_attr(trait_item: &syn::ItemTrait) -> bool {
    trait_item.attrs.iter().any(|attr| {
        if let syn::Meta::Path(path) = &attr.meta {
            path.is_ident("const_trait")
        } else {
            false
        }
    })
}