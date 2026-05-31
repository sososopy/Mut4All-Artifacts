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

pub struct Alter_Lifetime_Annotations_39;

impl Mutator for Alter_Lifetime_Annotations_39 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                let mut lifetimes = HashSet::new();
                if let Some(bound_lifetimes) = &func.sig.generics.params.iter().filter_map(|param| {
                    if let syn::GenericParam::Lifetime(lifetime) = param {
                        Some(lifetime)
                    } else {
                        None
                    }
                }).collect::<Vec<_>>() {
                    lifetimes.extend(bound_lifetimes.iter().map(|lt| lt.lifetime.clone()));
                }

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                            if let Some(lifetime) = &type_ref.lifetime {
                                if rng.gen_bool(0.5) {
                                    // Remove existing lifetime
                                    type_ref.lifetime = None;
                                } else {
                                    // Change existing lifetime
                                    let new_lifetime = lifetimes.iter().choose(&mut rng).cloned().unwrap_or_else(|| Lifetime::new("'a", Span::call_site()));
                                    type_ref.lifetime = Some(new_lifetime);
                                }
                            } else {
                                // Add a new lifetime
                                let new_lifetime = lifetimes.iter().choose(&mut rng).cloned().unwrap_or_else(|| Lifetime::new("'a", Span::call_site()));
                                type_ref.lifetime = Some(new_lifetime);
                            }
                        }
                    }
                }

                // Introduce an unused lifetime
                if rng.gen_bool(0.5) {
                    let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                    func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(unused_lifetime)));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}