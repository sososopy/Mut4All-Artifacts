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

pub struct Replace_Generic_Lifetime_With_Concrete_Lifetime_382;

impl Mutator for Replace_Generic_Lifetime_With_Concrete_Lifetime_382 {
    fn name(&self) -> &str {
        "Replace_Generic_Lifetime_With_Concrete_Lifetime_382"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            *lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            *lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            *lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime::new("'static", Span::call_site()),
                                colon_token: lifetime_param.colon_token.clone(),
                                bounds: lifetime_param.bounds.clone(),
                            };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic lifetime parameters in structs, impls, and functions with concrete lifetime parameters, such as 'static. This transformation tests the compiler's handling of lifetimes and generic const expressions, potentially exposing bugs in lifetime handling and const evaluation."
    }
}