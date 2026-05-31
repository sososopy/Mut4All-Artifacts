use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Change_Lifetime_Parameters_In_Implicit_Bounds_153;

impl Mutator for Change_Lifetime_Parameters_In_Implicit_Bounds_153 {
    fn name(&self) -> &str {
        "Change_Lifetime_Parameters_In_Implicit_Bounds_153"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = **return_type {
                        if let Some(bounds) = type_impl_trait.bounds.as_ref() {
                            let mut new_bounds = bounds.clone();
                            for bound in &mut new_bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = trait_bound.lifetimes.as_ref() {
                                        for lifetime in &mut lifetimes.lifetimes {
                                            if let GenericParam::Lifetime(lifetime_param) = lifetime {
                                                if let Some(lifetime_ident) = lifetime_param.lifetime.as_ref() {
                                                    if lifetime_ident.to_string() == "a" {
                                                        *lifetime = GenericParam::Lifetime(LifetimeParam {
                                                            attrs: lifetime_param.attrs.clone(),
                                                            lifetime: Lifetime {
                                                                apostrophe: lifetime_ident.apostrophe,
                                                                ident: Ident::new("b", lifetime_ident.span()),
                                                            },
                                                            colon_token: lifetime_param.colon_token,
                                                            bounds: lifetime_param.bounds.clone(),
                                                        });
                                                    } else if lifetime_ident.to_string() == "b" {
                                                        *lifetime = GenericParam::Lifetime(LifetimeParam {
                                                            attrs: lifetime_param.attrs.clone(),
                                                            lifetime: Lifetime {
                                                                apostrophe: lifetime_ident.apostrophe,
                                                                ident: Ident::new("a", lifetime_ident.span()),
                                                            },
                                                            colon_token: lifetime_param.colon_token,
                                                            bounds: lifetime_param.bounds.clone(),
                                                        });
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            type_impl_trait.bounds = Some(new_bounds);
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = **return_type {
                                if let Some(bounds) = type_impl_trait.bounds.as_ref() {
                                    let mut new_bounds = bounds.clone();
                                    for bound in &mut new_bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(lifetimes) = trait_bound.lifetimes.as_ref() {
                                                for lifetime in &mut lifetimes.lifetimes {
                                                    if let GenericParam::Lifetime(lifetime_param) = lifetime {
                                                        if let Some(lifetime_ident) = lifetime_param.lifetime.as_ref() {
                                                            if lifetime_ident.to_string() == "a" {
                                                                *lifetime = GenericParam::Lifetime(LifetimeParam {
                                                                    attrs: lifetime_param.attrs.clone(),
                                                                    lifetime: Lifetime {
                                                                        apostrophe: lifetime_ident.apostrophe,
                                                                        ident: Ident::new("b", lifetime_ident.span()),
                                                                    },
                                                                    colon_token: lifetime_param.colon_token,
                                                                    bounds: lifetime_param.bounds.clone(),
                                                                });
                                                            } else if lifetime_ident.to_string() == "b" {
                                                                *lifetime = GenericParam::Lifetime(LifetimeParam {
                                                                    attrs: lifetime_param.attrs.clone(),
                                                                    lifetime: Lifetime {
                                                                        apostrophe: lifetime_ident.apostrophe,
                                                                        ident: Ident::new("a", lifetime_ident.span()),
                                                                    },
                                                                    colon_token: lifetime_param.colon_token,
                                                                    bounds: lifetime_param.bounds.clone(),
                                                                });
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    type_impl_trait.bounds = Some(new_bounds);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return `impl Trait`. It changes the lifetime parameters in the implied bounds of the trait definition, swapping 'a and 'b lifetimes. This transformation tests the compiler's handling of lifetime substitutions and implicit bounds in trait definitions."
    }
}