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

pub struct Incorrect_Lifetime_Specification_109;

impl Mutator for Incorrect_Lifetime_Specification_109 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Specification_109"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_lifetime = false;
                let mut new_inputs = item_fn.sig.inputs.clone();

                for input in &mut new_inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &mut *pat_type.ty {
                            if let Some(lifetime) = &type_reference.lifetime {
                                has_lifetime = true;
                                type_reference.lifetime = Some(Lifetime::new("'_", lifetime.span()));
                            }
                        }
                    }
                }

                if has_lifetime {
                    item_fn.sig.inputs = new_inputs;
                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        if let Type::Reference(type_reference) = &mut **return_type {
                            if type_reference.lifetime.is_some() {
                                type_reference.lifetime = Some(Lifetime::new("'_", Span::call_site()));
                            }
                        }
                    }
                } else {
                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        if let Type::Reference(type_reference) = &mut **return_type {
                            if type_reference.lifetime.is_none() {
                                type_reference.lifetime = Some(Lifetime::new("'static", Span::call_site()));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with lifetime parameters. It replaces explicit lifetimes with anonymous ones ('_'), or inserts incorrect 'static lifetimes when none are specified. This explores compiler behavior under incorrect lifetime specifications."
    }
}