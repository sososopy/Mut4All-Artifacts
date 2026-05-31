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

pub struct Replace_Lifetime_Parameters_163;

impl Mutator for Replace_Lifetime_Parameters_163 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameters_163"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime = Ident::new(&format!("'new_{}", lifetime_param.lifetime.ident), lifetime_param.lifetime.ident.span());
                            lifetime_param.lifetime.ident = new_lifetime;
                        }
                    }
                }
            } else if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime = Ident::new(&format!("'new_{}", lifetime_param.lifetime.ident), lifetime_param.lifetime.ident.span());
                            lifetime_param.lifetime.ident = new_lifetime;
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime = Ident::new(&format!("'new_{}", lifetime_param.lifetime.ident), lifetime_param.lifetime.ident.span());
                            lifetime_param.lifetime.ident = new_lifetime;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetime parameters in trait and struct definitions with new lifetime parameters. This transformation tests the compiler's handling of lifetime checking and substitution, particularly in the context of impl trait."
    }
}