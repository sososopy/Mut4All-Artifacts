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

pub struct Replace_Lifetime_With_Static_383;

impl Mutator for Replace_Lifetime_With_Static_383 {
    fn name(&self) -> &str {
        "Replace_Lifetime_With_Static_383"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                for param in &mut s.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param.lifetime.ident = Ident::new("static", lifetime_param.lifetime.ident.span());
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                let mut new_params = Punctuated::new();
                for param in &impl_item.generics.params {
                    if let syn::GenericParam::Lifetime(_) = param {
                        // Skip lifetime parameters
                    } else {
                        new_params.push(param.clone());
                    }
                }
                impl_item.generics.params = new_params;
            }
        }
        // Add the feature attribute
        file.attrs.push(parse_quote! { #![feature(generic_const_exprs)] });
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}