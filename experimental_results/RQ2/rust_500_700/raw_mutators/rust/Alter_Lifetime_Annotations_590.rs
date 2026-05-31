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

pub struct Alter_Lifetime_Annotations_590;

impl Mutator for Alter_Lifetime_Annotations_590 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_590"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut lifetimes = HashSet::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        lifetimes.insert(lifetime.lifetime.clone());
                    }
                }

                if lifetimes.len() > 1 {
                    let first_lifetime = lifetimes.iter().next().unwrap().clone();
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                if let Some(lifetime) = &type_ref.lifetime {
                                    if lifetimes.contains(lifetime) {
                                        type_ref.lifetime = Some(first_lifetime.clone());
                                    }
                                }
                            }
                        }
                    }

                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Reference(type_ref) = &mut **return_type {
                            if let Some(lifetime) = &type_ref.lifetime {
                                if lifetimes.contains(lifetime) {
                                    type_ref.lifetime = Some(first_lifetime.clone());
                                }
                            }
                        }
                    }
                } else if lifetimes.len() == 1 {
                    let new_lifetime = Lifetime::new("'c", Span::call_site());
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                type_ref.lifetime = Some(new_lifetime.clone());
                            }
                        }
                    }

                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Reference(type_ref) = &mut **return_type {
                            type_ref.lifetime = Some(new_lifetime.clone());
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