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

pub struct Increase_Nested_Binder_Depth_378;

impl Mutator for Increase_Nested_Binder_Depth_378 {
    fn name(&self) -> &str {
        "Increase_Nested_Binder_Depth_378"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            if let syn::Type::Path(type_path) = type_pred.bounded_ty {
                                for segment in &mut type_path.path.segments {
                                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::AssocType(assoc_type) = arg {
                                                if let syn::Type::Trait(trait_bound) = assoc_type.ty.as_mut() {
                                                    if trait_bound.lifetimes.is_some() {
                                                        let new_lt = syn::Lifetime::new("'c", proc_macro2::Span::call_site());
                                                        let mut new_args = Punctuated::new();
                                                        for existing_arg in &args.args {
                                                            new_args.push(existing_arg.clone());
                                                        }
                                                        new_args.push(syn::GenericArgument::Lifetime(new_lt.clone()));
                                                        let new_ty = parse_quote! {
                                                            for<#new_lt> TraitC<#new_args>
                                                        };
                                                        let new_assoc_type = syn::AssocType {
                                                            ident: Ident::new("AsB", proc_macro2::Span::call_site()),
                                                            colon_token: Some(Default::default()),
                                                            ty: Box::new(new_ty),
                                                            eq_token: None,
                                                        };
                                                        args.args.push(syn::GenericArgument::AssocType(new_assoc_type));
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
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(type_pred) = predicate {
                                    if let syn::Type::Path(type_path) = type_pred.bounded_ty {
                                        for segment in &mut type_path.path.segments {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let syn::GenericArgument::AssocType(assoc_type) = arg {
                                                        if let syn::Type::Trait(trait_bound) = assoc_type.ty.as_mut() {
                                                            if trait_bound.lifetimes.is_some() {
                                                                let new_lt = syn::Lifetime::new("'c", proc_macro2::Span::call_site());
                                                                let mut new_args = Punctuated::new();
                                                                for existing_arg in &args.args {
                                                                    new_args.push(existing_arg.clone());
                                                                }
                                                                new_args.push(syn::GenericArgument::Lifetime(new_lt.clone()));
                                                                let new_ty = parse_quote! {
                                                                    for<#new_lt> TraitC<#new_args>
                                                                };
                                                                let new_assoc_type = syn::AssocType {
                                                                    ident: Ident::new("AsB", proc_macro2::Span::call_site()),
                                                                    colon_token: Some(Default::default()),
                                                                    ty: Box::new(new_ty),
                                                                    eq_token: None,
                                                                };
                                                                args.args.push(syn::GenericArgument::AssocType(new_assoc_type));
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