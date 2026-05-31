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

pub struct Impl_Trait_For_TypeAlias_486;

impl Mutator for Impl_Trait_For_TypeAlias_486 {
    fn name(&self) -> &str {
        "Impl_Trait_For_TypeAlias_486"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = vec![];
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                traits.push(trait_item.clone());
            }
        }

        if traits.is_empty() {
            return;
        }

        let mut insertions = vec![];
        for (i, item) in file.items.iter().enumerate() {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(_) = &item_type.def {
                    insertions.push((i, item_type.ident.clone()));
                }
            }
        }

        if insertions.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        for &(i, ref alias_ident) in insertions.iter().rev() {
            let selected_trait = traits.choose(&mut rng).unwrap();
            let trait_path = selected_trait.ident.clone();
            let new_impl = parse_quote! {
                impl #trait_path for #alias_ident {}
            };
            file.items.insert(i + 1, syn::Item::Impl(new_impl));
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}