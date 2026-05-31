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

pub struct Add_Conflicting_Trait_Impl_263;

impl Mutator for Add_Conflicting_Trait_Impl_263 {
    fn name(&self) -> &str {
        "Add_Conflicting_Trait_Impl_263"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_traits = Vec::new();
        let mut processed_traits = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let trait_ident = &trait_def.ident;
                for trait_item in &trait_def.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        if !trait_type.bounds.is_empty() {
                            target_traits.push((
                                trait_ident.clone(),
                                trait_type.ident.clone(),
                                trait_type.bounds.clone(),
                            ));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some(trait_ref) = &impl_item.trait_ {
                    let trait_path_segments = &trait_ref.path.segments;
                    if let Some(last_segment) = trait_path_segments.last() {
                        for (trait_ident, associated_type_name, _) in &target_traits {
                            if last_segment.ident == *trait_ident {
                                if !processed_traits.contains(trait_ident) {
                                    let new_impl = parse_quote! {
                                        impl #trait_ref for i32 {
                                            type #associated_type_name = i32;
                                        }
                                    };
                                    file.items.push(syn::Item::Impl(new_impl));
                                    processed_traits.insert(trait_ident.clone());
                                }
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