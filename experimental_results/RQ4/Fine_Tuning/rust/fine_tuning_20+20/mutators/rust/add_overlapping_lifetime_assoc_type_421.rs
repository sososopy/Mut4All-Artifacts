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

pub struct Add_Overlapping_Lifetime_Assoc_Type_421;

impl Mutator for Add_Overlapping_Lifetime_Assoc_Type_421 {
    fn name(&self) -> &str {
        "Add_Overlapping_Lifetime_Assoc_Type_421"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_structs = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if !item_struct.generics.params.is_empty() {
                    target_structs.insert(item_struct.ident.clone());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    continue;
                }
                let mut target = false;
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    if let Some(ident) = type_path.path.get_ident() {
                        if target_structs.contains(ident) {
                            target = true;
                        }
                    }
                }
                if !target {
                    continue;
                }
                let mut has_assoc = false;
                for impl_item in &item_impl.items {
                    if let ImplItem::Type(_) = impl_item {
                        has_assoc = true;
                        break;
                    }
                }
                if !has_assoc {
                    continue;
                }
                let mut new_items = Vec::new();
                for impl_item in &item_impl.items {
                    new_items.push(impl_item.clone());
                    if let ImplItem::Type(item_type) = impl_item {
                        if let Some(lifetimes) = &item_type.generics.params.iter().find_map(
                            |param| {
                                if let GenericParam::Lifetime(param) = param {
                                    Some(param)
                                } else {
                                    None
                                }
                            },
                        ) {
                            let new_assoc: ImplItemType = parse_quote! {
                                type NewAssoc = (#lifetimes, &'b ());
                            };
                            new_items.push(syn::ImplItem::Type(new_assoc));
                        }
                    }
                }
                item_impl.items = new_items;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets impl blocks for structs with generic parameters that define an associated type using lifetimes. It adds a new associated type with overlapping lifetimes, specifically introducing a new lifetime parameter that depends on an existing one. This transformation leverages complex lifetime interactions and associated type definitions to stress-test the compiler's lifetime resolution and type checking systems, aiming to reveal internal inconsistencies or crashes."
    }
}