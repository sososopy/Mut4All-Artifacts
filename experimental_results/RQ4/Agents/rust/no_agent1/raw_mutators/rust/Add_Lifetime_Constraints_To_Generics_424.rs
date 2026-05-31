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

pub struct Add_Lifetime_Constraints_To_Generics_424;

impl Mutator for Add_Lifetime_Constraints_To_Generics_424 {
    fn name(&self) -> &str {
        "Add_Lifetime_Constraints_To_Generics_424"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime = Lifetime::new("'a", Span::call_site());
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            type_param.bounds.push(TypeParamBound::Lifetime(lifetime.clone()));
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                let generics = &mut item_impl.generics;
                let lifetime = Lifetime::new("'a", Span::call_site());
                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        type_param.bounds.push(TypeParamBound::Lifetime(lifetime.clone()));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a lifetime parameter `'a` to function and impl generics, and applies it as a bound to existing type parameters. This transformation introduces complex lifetime constraints, potentially triggering lifetime resolution issues, borrow checker failures, or ICEs due to intricate lifetime interactions."
    }
}