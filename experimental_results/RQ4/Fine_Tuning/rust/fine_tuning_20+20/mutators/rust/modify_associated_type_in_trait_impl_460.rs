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

pub struct Modify_Associated_Type_In_Trait_Impl_460;

impl Mutator for Modify_Associated_Type_In_Trait_Impl_460 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_In_Trait_Impl_460"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut associated_types = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Type(item_type) = item {
                        associated_types.insert(item_type.ident.to_string());
                    }
                }
            }
        }
        let mut existing_types = HashSet::new();
        for item in &file.items {
            match item {
                Item::Struct(item_struct) => {
                    existing_types.insert(item_struct.ident.to_string());
                }
                Item::Enum(item_enum) => {
                    existing_types.insert(item_enum.ident.to_string());
                }
                Item::Type(item_type) => {
                    existing_types.insert(item_type.ident.to_string());
                }
                Item::Trait(item_trait) => {
                    existing_types.insert(item_trait.ident.to_string());
                }
                _ => {}
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut target_idx = None;
                for (idx, item) in item_impl.items.iter().enumerate() {
                    if let ImplItem::Type(item_type) = item {
                        if associated_types.contains(&item_type.ident.to_string()) {
                            target_idx = Some(idx);
                            break;
                        }
                    }
                }
                if let Some(idx) = target_idx {
                    if let ImplItem::Type(item_type) = &mut item_impl.items[idx] {
                        let mut rng = thread_rng();
                        let mut candidates: Vec<_> = existing_types
                            .iter()
                            .filter(|ty| **ty != item_type.ty.to_token_stream().to_string())
                            .collect();
                        candidates.shuffle(&mut rng);
                        if let Some(new_ty) = candidates.into_iter().next() {
                            item_type.ty = parse_str(new_ty).unwrap();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify traits with associated types and their implementations. Replace the associated type in the implementation with a different existing type from the seed program, aiming to disrupt specialization coherence and trigger compiler errors."
    }
}