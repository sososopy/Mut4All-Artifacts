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

pub struct Modify_Const_Generic_Lifetime_In_Fn_103;

impl Mutator for Modify_Const_Generic_Lifetime_In_Fn_103 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Lifetime_In_Fn_103"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_const_generic_with_lifetime = false;
                let mut existing_lifetimes = HashSet::new();
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        existing_lifetimes.insert(lifetime.lifetime.ident.to_string());
                    }
                }
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = const_param.ty.as_ref() {
                            has_const_generic_with_lifetime = true;
                            if let Some(lifetime) = &type_ref.lifetime {
                                existing_lifetimes
                                    .insert(lifetime.ident.to_string());
                            }
                        }
                    }
                }
                if !has_const_generic_with_lifetime {
                    continue;
                }
                let mut new_lifetime_ident = None;
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = const_param.ty.as_ref() {
                            if let Some(lifetime) = &type_ref.lifetime {
                                for existing in &existing_lifetimes {
                                    if existing != &lifetime.ident.to_string() {
                                        new_lifetime_ident = Some(existing.clone());
                                        break;
                                    }
                                }
                                if new_lifetime_ident.is_none() {
                                    new_lifetime_ident = Some("'a".to_string());
                                }
                            } else {
                                new_lifetime_ident = Some("'a".to_string());
                            }
                        }
                    }
                }
                let new_lifetime_ident = new_lifetime_ident.unwrap_or_else(|| "'a".to_string());
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = const_param.ty.as_mut() {
                            type_ref.lifetime = Some(Lifetime::new(&new_lifetime_ident, Span::call_site()));
                        }
                    }
                }
                if !existing_lifetimes.contains(&new_lifetime_ident) {
                    item_fn.sig.generics.params.insert(
                        0,
                        GenericParam::Lifetime(LifetimeParam::new(Lifetime::new(
                            &new_lifetime_ident,
                            Span::call_site(),
                        ))),
                    );
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify function signatures using const generics with lifetimes, modify the lifetime specifier by changing it to another existing lifetime or introducing a new one, and ensure the transformation maintains syntactic validity and consistency within the code."
    }
}