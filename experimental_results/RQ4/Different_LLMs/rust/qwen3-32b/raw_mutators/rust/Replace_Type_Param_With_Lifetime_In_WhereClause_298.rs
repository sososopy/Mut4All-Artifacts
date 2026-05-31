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

pub struct Replace_Type_Param_With_Lifetime_In_WhereClause_298;

impl Mutator for Replace_Type_Param_With_Lifetime_In_WhereClause_298 {
    fn name(&self) -> &str {
        "Replace_Type_Param_With_Lifetime_In_WhereClause_298"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(trait_type) = trait_item {
                        let assoc_type_name = &trait_type.ident;
                        let mut assoc_type_generics = Vec::new();
                        for param in &trait_type.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                assoc_type_generics.push((&type_param.ident, param.span()));
                            }
                        }
                        if !assoc_type_generics.is_empty() {
                            for method in &mut trait_item.items {
                                if let syn::TraitItem::Fn(method) = method {
                                    if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                        if let syn::Type::Path(type_path) = &**return_type {
                                            let path = &type_path.path;
                                            if let [first, second] = path.segments.as_slice() {
                                                if first.ident == "Self" && second.ident == *assoc_type_name {
                                                    if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                                        for predicate in &mut where_clause.predicates {
                                                            if let syn::WherePredicate::Type(type_pred) = predicate {
                                                                if let syn::Type::Path(type_path) = &*type_pred.bounded_ty {
                                                                    if type_path.path.segments.len() == 1 {
                                                                        let seg = &type_path.path.segments[0];
                                                                        let param_name = &seg.ident;
                                                                        if assoc_type_generics.iter().any(|(name, _)| name == param_name) {
                                                                            let mut lifetimes = Vec::new();
                                                                            for input in &method.sig.inputs {
                                                                                if let syn::FnArg::Receiver(receiver) = input {
                                                                                    if let Some(lifetime) = &receiver.lifetime {
                                                                                        lifetimes.push(lifetime.clone());
                                                                                    }
                                                                                } else if let syn::FnArg::Typed(pat_type) = input {
                                                                                    if let syn::Type::Reference(ref_type) = &*pat_type.ty {
                                                                                        if let Some(lifetime) = &ref_type.lifetime {
                                                                                            lifetimes.push(lifetime.clone());
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                            if let Some(chosen_lifetime) = lifetimes.first() {
                                                                                let new_type = syn::Type::Lifetime(syn::Lifetime {
                                                                                    apostrophe: proc_macro2::Token!['](proc_macro2::Span::call_site()),
                                                                                    ident: chosen_lifetime.ident.clone(),
                                                                                });
                                                                                *type_pred.bounded_ty = Box::new(new_type);
                                                                                for param in &mut trait_type.generics.params {
                                                                                    if let syn::GenericParam::Type(type_param) = param {
                                                                                        if type_param.ident == *param_name {
                                                                                            let new_param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                                                                lifetime: syn::Lifetime {
                                                                                                    apostrophe: proc_macro2::Token!['](proc_macro2::Span::call_site()),
                                                                                                    ident: chosen_lifetime.ident.clone(),
                                                                                                },
                                                                                                colon_token: type_param.colon_token,
                                                                                                bounds: type_param.bounds.clone(),
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(trait_type) = impl_item {
                        let assoc_type_name = &trait_type.ident;
                        let mut assoc_type_generics = Vec::new();
                        for param in &trait_type.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                assoc_type_generics.push((&type_param.ident, param.span()));
                            }
                        }
                        if !assoc_type_generics.is_empty() {
                            for method in &mut impl_item.items {
                                if let syn::ImplItem::Fn(method) = method {
                                    if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                        if let syn::Type::Path(type_path) = &**return_type {
                                            let path = &type_path.path;
                                            if let [first, second] = path.segments.as_slice() {
                                                if first.ident == "Self" && second.ident == *assoc_type_name {
                                                    if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                                        for predicate in &mut where_clause.predicates {
                                                            if let syn::WherePredicate::Type(type_pred) = predicate {
                                                                if let syn::Type::Path(type_path) = &*type_pred.bounded_ty {
                                                                    if type_path.path.segments.len() == 1 {
                                                                        let seg = &type_path.path.segments[0];
                                                                        let param_name = &seg.ident;
                                                                        if assoc_type_generics.iter().any(|(name, _)| name == param_name) {
                                                                            let mut lifetimes = Vec::new();
                                                                            for input in &method.sig.inputs {
                                                                                if let syn::FnArg::Receiver(receiver) = input {
                                                                                    if let Some(lifetime) = &receiver.lifetime {
                                                                                        lifetimes.push(lifetime.clone());
                                                                                    }
                                                                                } else if let syn::FnArg::Typed(pat_type) = input {
                                                                                    if let syn::Type::Reference(ref_type) = &*pat_type.ty {
                                                                                        if let Some(lifetime) = &ref_type.lifetime {
                                                                                            lifetimes.push(lifetime.clone());
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                            if let Some(chosen_lifetime) = lifetimes.first() {
                                                                                let new_type = syn::Type::Lifetime(syn::Lifetime {
                                                                                    apostrophe: proc_macro2::Token!['](proc_macro2::Span::call_site()),
                                                                                    ident: chosen_lifetime.ident.clone(),
                                                                                });
                                                                                *type_pred.bounded_ty = Box::new(new_type);
                                                                                for param in &mut trait_type.generics.params {
                                                                                    if let syn::GenericParam::Type(type_param) = param {
                                                                                        if type_param.ident == *param_name {
                                                                                            let new_param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                                                                lifetime: syn::Lifetime {
                                                                                                    apostrophe: proc_macro2::Token!['](proc_macro2::Span::call_site()),
                                                                                                    ident: chosen_lifetime.ident.clone(),
                                                                                                },
                                                                                                colon_token: type_param.colon_token,
                                                                                                bounds: type_param.bounds.clone(),
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}