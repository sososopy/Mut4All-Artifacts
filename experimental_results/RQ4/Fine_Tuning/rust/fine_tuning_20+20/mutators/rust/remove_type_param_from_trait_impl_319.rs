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

pub struct Remove_Type_Param_From_Trait_Impl_319;

impl Mutator for Remove_Type_Param_From_Trait_Impl_319 {
    fn name(&self) -> &str {
        "Remove_Type_Param_From_Trait_Impl_319"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_items: Vec<_> = file
            .items
            .iter()
            .filter_map(|item| {
                if let Item::Trait(item_trait) = item {
                    Some(item_trait.clone())
                } else {
                    None
                }
            })
            .collect();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let trait_ident = path.segments.last().unwrap().ident.to_string();
                    let mut target_trait = None;
                    for item_trait in &trait_items {
                        if item_trait.ident == trait_ident {
                            target_trait = Some(item_trait.clone());
                            break;
                        }
                    }
                    if let Some(target_trait) = target_trait {
                        let trait_type_params: HashSet<_> = target_trait
                            .generics
                            .params
                            .iter()
                            .filter_map(|param| {
                                if let GenericParam::Type(type_param) = param {
                                    Some(type_param.ident.to_string())
                                } else {
                                    None
                                }
                            })
                            .collect();
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let method_name = method.sig.ident.to_string();
                                let mut target_trait_method = None;
                                for item in &target_trait.items {
                                    if let TraitItem::Fn(trait_method) = item {
                                        if trait_method.sig.ident == method.sig.ident {
                                            target_trait_method = Some(trait_method.clone());
                                            break;
                                        }
                                    }
                                }
                                if let Some(target_trait_method) = target_trait_method {
                                    let impl_type_params: HashSet<_> = method
                                        .sig
                                        .generics
                                        .params
                                        .iter()
                                        .filter_map(|param| {
                                            if let GenericParam::Type(type_param) = param {
                                                Some(type_param.ident.to_string())
                                            } else {
                                                None
                                            }
                                        })
                                        .collect();
                                    let mut type_params_to_remove = Vec::new();
                                    for param in &method.sig.generics.params {
                                        if let GenericParam::Type(type_param) = param {
                                            if trait_type_params
                                                .contains(&type_param.ident.to_string())
                                                && !target_trait_method
                                                    .sig
                                                    .generics
                                                    .params
                                                    .iter()
                                                    .any(|trait_param| {
                                                        if let GenericParam::Type(trait_type_param) =
                                                            trait_param
                                                        {
                                                            trait_type_param.ident
                                                                == type_param.ident
                                                        } else {
                                                            false
                                                        }
                                                    })
                                            {
                                                type_params_to_remove.push(type_param.ident.clone());
                                            }
                                        }
                                    }
                                    method.sig.generics.params = method
                                        .sig
                                        .generics
                                        .params
                                        .iter()
                                        .cloned()
                                        .filter(|param| {
                                            if let GenericParam::Type(type_param) = param {
                                                !type_params_to_remove
                                                    .contains(&type_param.ident)
                                            } else {
                                                true
                                            }
                                        })
                                        .collect();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations where the trait has type parameters. It removes type parameters from the implementation's method signature that are present in the trait's type parameter list but not used in the trait method signature. This creates a mismatch between the trait definition and its implementation, potentially leading to internal compiler errors due to type parameter discrepancies."
    }
}