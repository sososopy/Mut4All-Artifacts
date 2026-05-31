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

pub struct Replace_Lifetime_Parameter_With_Invalid_Lifetime_Name_192;

impl Mutator for Replace_Lifetime_Parameter_With_Invalid_Lifetime_Name_192 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_With_Invalid_Lifetime_Name_192"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    continue;
                }
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        lifetime.lifetime.ident = Ident::new("static", Span::call_site());
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if impl_item.generics.params.is_empty() {
                    continue;
                }
                for param in &mut impl_item.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        lifetime.lifetime.ident = Ident::new("static", Span::call_site());
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid lifetime parameters with 'static in struct and impl definitions. This transformation introduces invalid lifetime parameters, potentially triggering errors related to lifetime handling, especially in the context of generic const expressions."
    }
}