use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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

pub struct Specialization_Overlap_Impl_455;

impl Mutator for Specialization_Overlap_Impl_455 {
    fn name(&self) -> &str {
        "Specialization_Overlap_Impl_455"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(trait_ref) = &item_impl.trait_ {
                    let has_assoc_type = item_impl.items.iter().any(|i| matches!(i, syn::ImplItem::Type(_)));
                    if !has_assoc_type {
                        continue;
                    }
                    let assoc_type_name = item_impl.items.iter()
                        .find_map(|i| {
                            if let syn::ImplItem::Type(ty) = i {
                                Some(ty.ident.to_string())
                            } else {
                                None
                            }
                        })
                        .unwrap_or_else(|| "Output".to_string());
                    for param in &mut item_impl.generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            let mut bounds = Punctuated::new();
                            bounds.push(parse_quote!(Clone));
                            type_param.bounds = bounds;
                        }
                    }
                    let generics = &item_impl.generics;
                    let trait_path = trait_ref.1.clone();
                    let new_impl = if !generics.params.is_empty() {
                        parse_quote! {
                            impl<#generics.params> #trait_path for u8 {
                                type #assoc_type_name = u16;
                            }
                        }
                    } else {
                        parse_quote! {
                            impl #trait_path for u8 {
                                type #assoc_type_name = u16;
                            }
                        }
                    };
                    let new_trait = parse_quote! {
                        trait Foo { }
                    };
                    let new_trait_impl = parse_quote! {
                        impl Foo for <u8 as #trait_path>::#assoc_type_name {}
                    };
                    new_items.push(syn::Item::Impl(new_impl));
                    new_items.push(syn::Item::Trait(new_trait));
                    new_items.push(syn::Item::Impl(new_trait_impl));
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}