use proc_macro2::{Span};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, ItemImpl, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Type_Param_With_Lifetime_In_WhereClause_298;

impl Mutator for Replace_Type_Param_With_Lifetime_In_WhereClause_298 {
    fn name(&self) -> &str {
        "Replace_Type_Param_With_Lifetime_In_WhereClause_298"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut assoc_type_name = None;
                let mut assoc_type_generics = Vec::new();
                let mut assoc_trait_type: Option<&mut syn::TraitItemType> = None;
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        assoc_type_name = Some(&trait_type.ident);
                        assoc_trait_type = Some(trait_type);
                        for param in &trait_type.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                assoc_type_generics.push((&type_param.ident, param.span()));
                            }
                        }
                    }
                }
                if let Some(assoc_type_name) = assoc_type_name {
                    for method in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = method {
                            if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                if let syn::Type::Path(type_path) = &**return_type {
                                    let path = &type_path.path;
                                    if path.segments.len() == 2 {
                                        let first = &path.segments[0];
                                        let second = &path.segments[1];
                                        if first.ident == "Self" && second.ident == *assoc_type_name {
                                            if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                                for predicate in &mut where_clause.predicates {
                                                    if let syn::WherePredicate::Type(type_pred) = predicate {
                                                        if let syn::Type::Path(type_path) = &**type_pred.bounded_ty {
                                                            if type_path.path.segments.len() == 1 {
                                                                let seg = &type_path.path.segments[0];
                                                                let param_name = &seg.ident;
                                                                if assoc_type_generics.iter().any(|(name, _)| *name == param_name) {
                                                                    let mut lifetimes = Vec::new();
                                                                    for input in &method.sig.inputs {
                                                                        if let syn::FnArg::Receiver(receiver) = input {
                                                                            if let Some(lifetime) = receiver.lifetime() {
                                                                                lifetimes.push(lifetime.clone());
                                                                            }
                                                                        } else if let syn::FnArg::Typed(pat_type) = input {
                                                                            if let syn::Type::Reference(ref_type) = &*pat_type.ty {
                                                                                if let Some(lifetime) = ref_type.lifetime {
                                                                                    lifetimes.push(lifetime.clone());
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    if let Some(chosen_lifetime) = lifetimes.first() {
                                                                        let new_type = syn::Type::Reference(syn::TypeReference {
                                                                            and_token: syn::token::And::default(),
                                                                            lifetime: Some(chosen_lifetime.clone()),
                                                                            mutability: None,
                                                                            elem: Box::new(syn::Type::Infer(syn::TypeInfer {
                                                                                underscore_token: syn::token::Underscore::default(),
                                                                            })),
                                                                        });
                                                                        *type_pred.bounded_ty = Box::new(new_type);
                                                                        if let Some(assoc_trait_type) = assoc_trait_type {
                                                                            for param in &mut assoc_trait_type.generics.params {
                                                                                if let syn::GenericParam::Type(type_param) = param {
                                                                                    if type_param.ident == *param_name {
                                                                                        let new_param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                                                            attrs: Default::default(),
                                                                                            lifetime: syn::Lifetime {
                                                                                                apostrophe: chosen_lifetime.apostrophe.clone(),
                                                                                                ident: chosen_lifetime.ident.clone(),
                                                                                            },
                                                                                            colon_token: type_param.colon_token,
                                                                                            bounds: Punctuated::new(),
                                                                                        });
                                                                                        *param = new_param;
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
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                let mut assoc_type_name = None;
                let mut assoc_type_generics = Vec::new();
                let mut assoc_impl_type: Option<&mut syn::ImplItemType> = None;
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(trait_type) = impl_item {
                        assoc_type_name = Some(&trait_type.ident);
                        assoc_impl_type = Some(trait_type);
                        for param in &trait_type.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                assoc_type_generics.push((&type_param.ident, param.span()));
                            }
                        }
                    }
                }
                if let Some(assoc_type_name) = assoc_type_name {
                    for method in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = method {
                            if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                if let syn::Type::Path(type_path) = &**return_type {
                                    let path = &type_path.path;
                                    if path.segments.len() == 2 {
                                        let first = &path.segments[0];
                                        let second = &path.segments[1];
                                        if first.ident == "Self" && second.ident == *assoc_type_name {
                                            if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                                for predicate in &mut where_clause.predicates {
                                                    if let syn::WherePredicate::Type(type_pred) = predicate {
                                                        if let syn::Type::Path(type_path) = &**type_pred.bounded_ty {
                                                            if type_path.path.segments.len() == 1 {
                                                                let seg = &type_path.path.segments[0];
                                                                let param_name = &seg.ident;
                                                                if assoc_type_generics.iter().any(|(name, _)| *name == param_name) {
                                                                    let mut lifetimes = Vec::new();
                                                                    for input in &method.sig.inputs {
                                                                        if let syn::FnArg::Receiver(receiver) = input {
                                                                            if let Some(lifetime) = receiver.lifetime() {
                                                                                lifetimes.push(lifetime.clone());
                                                                            }
                                                                        } else if let syn::FnArg::Typed(pat_type) = input {
                                                                            if let syn::Type::Reference(ref_type) = &*pat_type.ty {
                                                                                if let Some(lifetime) = ref_type.lifetime {
                                                                                    lifetimes.push(lifetime.clone());
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    if let Some(chosen_lifetime) = lifetimes.first() {
                                                                        let new_type = syn::Type::Reference(syn::TypeReference {
                                                                            and_token: syn::token::And::default(),
                                                                            lifetime: Some(chosen_lifetime.clone()),
                                                                            mutability: None,
                                                                            elem: Box::new(syn::Type::Infer(syn::TypeInfer {
                                                                                underscore_token: syn::token::Underscore::default(),
                                                                            })),
                                                                        });
                                                                        *type_pred.bounded_ty = Box::new(new_type);
                                                                        if let Some(assoc_impl_type) = assoc_impl_type {
                                                                            for param in &mut assoc_impl_type.generics.params {
                                                                                if let syn::GenericParam::Type(type_param) = param {
                                                                                    if type_param.ident == *param_name {
                                                                                        let new_param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                                                            attrs: Default::default(),
                                                                                            lifetime: syn::Lifetime {
                                                                                                apostrophe: chosen_lifetime.apostrophe.clone(),
                                                                                                ident: chosen_lifetime.ident.clone(),
                                                                                            },
                                                                                            colon_token: type_param.colon_token,
                                                                                            bounds: Punctuated::new(),
                                                                                        });
                                                                                        *param = new_param;
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
        ""
    }
}