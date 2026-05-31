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

pub struct Vary_Trait_Bound_Lifetimes_636;

impl Mutator for Vary_Trait_Bound_Lifetimes_636 {
    fn name(&self) -> &str {
        "Vary_Trait_Bound_Lifetimes_636"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut additional_lifetime = None;
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if additional_lifetime.is_none() {
                            additional_lifetime = Some(Lifetime::new("'b", Span::call_site()));
                            trait_item.generics.params.push(GenericParam::Lifetime(
                                LifetimeParam::new(additional_lifetime.clone().unwrap()),
                            ));
                        }
                    }
                }

                if let Some(additional_lifetime) = additional_lifetime {
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Type(type_item) = item {
                            type_item.bounds.push(TypeParamBound::Lifetime(additional_lifetime.clone()));
                        }
                        if let syn::TraitItem::Method(method) = item {
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Reference(type_reference) = &mut *pat_type.ty {
                                        type_reference.lifetime = Some(additional_lifetime.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                let mut additional_lifetime = None;
                for param in &mut impl_item.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if additional_lifetime.is_none() {
                            additional_lifetime = Some(Lifetime::new("'b", Span::call_site()));
                            impl_item.generics.params.push(GenericParam::Lifetime(
                                LifetimeParam::new(additional_lifetime.clone().unwrap()),
                            ));
                        }
                    }
                }

                if let Some(additional_lifetime) = additional_lifetime {
                    for item in &mut impl_item.items {
                        if let syn::ImplItem::Type(type_item) = item {
                            type_item.generics.params.push(GenericParam::Lifetime(
                                LifetimeParam::new(additional_lifetime.clone()),
                            ));
                        }
                        if let syn::ImplItem::Method(method) = item {
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Reference(type_reference) = &mut *pat_type.ty {
                                        type_reference.lifetime = Some(additional_lifetime.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an additional lifetime parameter to trait and impl blocks that use lifetime parameters. It adjusts associated types and function parameters to use this new lifetime, aiming to test the compiler's handling of complex lifetime dependencies and trait obligations."
    }
}