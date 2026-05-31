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

pub struct Modify_Associated_Type_Lifetime_411;

impl Mutator for Modify_Associated_Type_Lifetime_411 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Lifetime_411"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut associated_types = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Type(item_type) = item {
                        associated_types.insert(item_type.ident.to_string());
                    }
                    if let TraitItem::Fn(item_fn) = item {
                        if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                if type_path.qself.is_some() {
                                    associated_types.insert(type_path.path.segments[0].ident.to_string());
                                }
                            }
                        }
                    }
                }
            }
        }
        let mut target_impls = Vec::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.len() == 1 && associated_types.contains(&path.segments[0].ident.to_string()) {
                        target_impls.push(index);
                    }
                }
            }
        }
        for index in target_impls {
            if let Item::Impl(item_impl) = &mut file.items[index] {
                let mut impl_lifetimes = Vec::new();
                for param in &item_impl.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        impl_lifetimes.push(lifetime.lifetime.ident.to_string());
                    }
                }
                for item in &mut item_impl.items {
                    if let ImplItem::Type(item_type) = item {
                        if let Type::Path(type_path) = &mut item_type.ty {
                            if type_path.qself.is_some() {
                                if let Some(mut first_segment) = type_path.path.segments.first_mut() {
                                    if let PathArguments::AngleBracketed(angle_args) = &mut first_segment.arguments {
                                        for arg in &mut angle_args.args {
                                            if let GenericArgument::Lifetime(lifetime) = arg {
                                                let lifetime_name = lifetime.ident.to_string();
                                                if impl_lifetimes.contains(&lifetime_name) {
                                                    let new_lifetime = if lifetime_name == "d" {
                                                        "'cursor"
                                                    } else {
                                                        "'d"
                                                    };
                                                    *arg = GenericArgument::Lifetime(Lifetime::new(new_lifetime, lifetime.span()));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify traits with associated types or functions returning associated types, locate corresponding impl blocks, and modify the lifetime parameters in associated type definitions to create conflicts with existing lifetimes, aiming to trigger lifetime resolution issues and potential ICEs."
    }
}