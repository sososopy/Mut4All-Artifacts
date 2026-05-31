use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Generic_Type_Parameters_With_Concrete_Types_420;

impl Mutator for Replace_Generic_Type_Parameters_With_Concrete_Types_420 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameters_With_Concrete_Types_420"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::WhereClause(_) = &struct_item.generics {
                    let mut new_generics = syn::Generics::default();
                    for param in struct_item.generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            let concrete_type = match thread_rng().gen_range(0..4) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                3 => parse_quote!(Option<i32>),
                                _ => unreachable!(),
                            };
                            new_generics.params.push(syn::GenericParam::Type(syn::TypeParam {
                                attrs: type_param.attrs.clone(),
                                ident: type_param.ident.clone(),
                                eq_token: type_param.eq_token,
                                colon_token: type_param.colon_token,
                                bounds: Punctuated::from_iter(vec![syn::TypeParamBound::Trait(
                                    syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                ident: Ident::new(
                                                    "Sized",
                                                    Span::call_site(),
                                                ),
                                                arguments: syn::PathArguments::None,
                                            }]),
                                        },
                                    },
                                )]),
                                default: Some(concrete_type),
                            }));
                        } else {
                            new_generics.params.push(param);
                        }
                    }
                    struct_item.generics = new_generics;
                }
            }

            if let syn::Item::Fn(func) = item {
                if let syn::Generics::WhereClause(_) = &func.sig.generics {
                    let mut new_generics = syn::Generics::default();
                    for param in func.sig.generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            let concrete_type = match thread_rng().gen_range(0..4) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                3 => parse_quote!(Option<i32>),
                                _ => unreachable!(),
                            };
                            new_generics.params.push(syn::GenericParam::Type(syn::TypeParam {
                                attrs: type_param.attrs.clone(),
                                ident: type_param.ident.clone(),
                                eq_token: type_param.eq_token,
                                colon_token: type_param.colon_token,
                                bounds: Punctuated::from_iter(vec![syn::TypeParamBound::Trait(
                                    syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                ident: Ident::new(
                                                    "Sized",
                                                    Span::call_site(),
                                                ),
                                                arguments: syn::PathArguments::None,
                                            }]),
                                        },
                                    },
                                )]),
                                default: Some(concrete_type),
                            }));
                        } else {
                            new_generics.params.push(param);
                        }
                    }
                    func.sig.generics = new_generics;
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let syn::Generics::WhereClause(_) = &impl_item.generics {
                    let mut new_generics = syn::Generics::default();
                    for param in impl_item.generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            let concrete_type = match thread_rng().gen_range(0..4) {
                                0 => parse_quote!(i32),
                                1 => parse_quote!(String),
                                2 => parse_quote!(Vec<i32>),
                                3 => parse_quote!(Option<i32>),
                                _ => unreachable!(),
                            };
                            new_generics.params.push(syn::GenericParam::Type(syn::TypeParam {
                                attrs: type_param.attrs.clone(),
                                ident: type_param.ident.clone(),
                                eq_token: type_param.eq_token,
                                colon_token: type_param.colon_token,
                                bounds: Punctuated::from_iter(vec![syn::TypeParamBound::Trait(
                                    syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                ident: Ident::new(
                                                    "Sized",
                                                    Span::call_site(),
                                                ),
                                                arguments: syn::PathArguments::None,
                                            }]),
                                        },
                                    },
                                )]),
                                default: Some(concrete_type),
                            }));
                        } else {
                            new_generics.params.push(param);
                        }
                    }
                    impl_item.generics = new_generics;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic type parameters with concrete types (i32, String, Vec<i32>, Option<i32>) in structs, functions, and impl blocks. This transformation aims to test the compiler's ability to handle type checking and substitution in different scenarios, particularly with inherent associated types."
    }
}