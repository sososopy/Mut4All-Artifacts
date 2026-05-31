use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Associated_Type_With_Concrete_Type_6;

impl Mutator for Replace_Associated_Type_With_Concrete_Type_6 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_With_Concrete_Type_6"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_implementations = Vec::new();
        for item in &file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_path) = &impl_item.trait_ {
                    if let Some(path) = trait_path.1.as_ref() {
                        trait_implementations.push((impl_item.self_ty.clone(), path.clone()));
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Type(type_item) = item {
                        let associated_type = type_item.ident.clone();
                        if let Some(trait_bounds) = type_item.bounds.as_ref() {
                            for trait_bound in trait_bounds {
                                if let TypeParamBound::Trait(trait_bound) = trait_bound {
                                    for (implementing_type, implementing_trait_path) in &trait_implementations {
                                        if implementing_trait_path == &trait_bound.path {
                                            replace_associated_type_with_concrete_type(
                                                file,
                                                &associated_type,
                                                implementing_type.clone(),
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Rust code that utilizes associated types in traits. It replaces an associated type with a concrete type that implements the trait, focusing on the trait system and associated items. This approach aims to achieve high coverage across different program structures and features, particularly those related to Rust's trait system and type inference."
    }
}

fn get_trait_path(impl_item: &ItemImpl) -> Option<SynPath> {
    if let Some(trait_path) = &impl_item.trait_ {
        trait_path.1.clone()
    } else {
        None
    }
}

fn get_trait_path_from_item(item: &TraitItem) -> Option<SynPath> {
    if let TraitItem::Type(type_item) = item {
        if let Some(trait_bounds) = type_item.bounds.as_ref() {
            if let Some(trait_bound) = trait_bounds.iter().next() {
                if let TypeParamBound::Trait(trait_bound) = trait_bound {
                    Some(trait_bound.path.clone())
                } else {
                    None
                }
            } else {
                None
            }
        } else {
            None
        }
    } else {
        None
    }
}

fn replace_associated_type_with_concrete_type(
    file: &mut File,
    associated_type: &Ident,
    concrete_type: Type,
) {
    let mut new_file = file.clone();
    for item in &mut new_file.items {
        if let Item::Impl(impl_item) = item {
            for impl_item in &mut impl_item.items {
                if let ImplItem::Fn(func) = impl_item {
                    for arg in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.segments.last().unwrap().ident == *associated_type {
                                    pat_type.ty = Box::new(concrete_type.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    *file = new_file;
}