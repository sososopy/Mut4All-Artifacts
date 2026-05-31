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

pub struct Mismatched_Function_Return_Type_Versus_Generic_Parameter_150;

impl Mutator for Mismatched_Function_Return_Type_Versus_Generic_Parameter_150 {
    fn name(&self) -> &str {
        "Mismatched_Function_Return_Type_Versus_Generic_Parameter_150"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generic_params = &func.sig.generics.params;
                let mut generic_names = HashSet::new();
                for param in generic_params {
                    if let GenericParam::Type(type_param) = param {
                        generic_names.insert(type_param.ident.to_string());
                    }
                }
                if generic_names.contains("Vec") || generic_names.contains("HashMap") || generic_names.contains("String") {
                    if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                        if let Type::Path(type_path) = &**return_type {
                            if let Some(first_segment) = type_path.path.segments.first() {
                                if first_segment.ident == "HashMap" || first_segment.ident == "Vec" || first_segment.ident == "String" {
                                    for param_name in &generic_names {
                                        if param_name == "Vec" || param_name == "HashMap" || param_name == "String" {
                                            let new_return_type = syn::Type::Path(syn::TypePath {
                                                qself: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![
                                                        syn::PathSegment {
                                                            ident: syn::Ident::new(param_name, Span::call_site()),
                                                            arguments: syn::PathArguments::AngleBracketed(
                                                                syn::AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: Punctuated::from_iter(vec![
                                    syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(syn::Ident::new("HashMap", Span::call_site())),
                                    })),
                                ]),
                                                                gt_token: token::Gt::default(),
                                                            },
                                                        ),
                                                    },
                                                ]),
                                                },
                                            });
                                            *return_type = Box::new(new_return_type);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item_fn in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item_fn {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let generic_params = &func.sig.generics.params;
                        let mut generic_names = HashSet::new();
                        for param in generic_params {
                            if let GenericParam::Type(type_param) = param {
                                generic_names.insert(type_param.ident.to_string());
                            }
                        }
                        if generic_names.contains("Vec") || generic_names.contains("HashMap") || generic_names.contains("String") {
                            if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                                if let Type::Path(type_path) = &**return_type {
                                    if let Some(first_segment) = type_path.path.segments.first() {
                                        if first_segment.ident == "HashMap" || first_segment.ident == "Vec" || first_segment.ident == "String" {
                                            for param_name in &generic_names {
                                                if param_name == "Vec" || param_name == "HashMap" || param_name == "String" {
                                                    let new_return_type = syn::Type::Path(syn::TypePath {
                                                        qself: None,
                                                        path: syn::Path {
                                                            leading_colon: None,
                                                            segments: Punctuated::from_iter(vec![
                                                                syn::PathSegment {
                                    ident: syn::Ident::new(param_name, Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![
                                                syn::GenericArgument::Type(syn::TypePath {
                                                    qself: None,
                                                    path: syn::Path::from(syn::Ident::new("HashMap", Span::call_site())),
                                                }),
                                            ]),
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                },
                            ]),
                                                        },
                                                    });
                                                    *return_type = Box::new(new_return_type);
                                                    break;
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
        "The mutation operator targets function definitions where generic parameters shadow common type names (Vec, HashMap, String). It replaces the return type with a constructed path using the generic parameter as a constructor, e.g., Vec<HashMap>. This creates ambiguity between generic parameters and concrete types, potentially causing mismatched counts in generic argument inference and triggering compiler ICEs."
    }
}