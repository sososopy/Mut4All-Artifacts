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

pub struct Remove_Lifetime_Parameters_9;

impl Mutator for Remove_Lifetime_Parameters_9 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameters_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Trait(trait_item) => {
                    trait_item.generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Method(method) = item {
                            method.sig.generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                            if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                if let Type::Reference(type_reference) = &mut **ty {
                                    type_reference.lifetime = None;
                                }
                            }
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    struct_item.generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                    for field in &mut struct_item.fields {
                        if let Type::Reference(type_reference) = &mut field.ty {
                            type_reference.lifetime = None;
                        }
                    }
                }
                Item::Fn(func) => {
                    func.sig.generics.params.retain(|param| !matches!(param, GenericParam::Lifetime(_)));
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_reference) = &mut *pat_type.ty {
                                type_reference.lifetime = None;
                            }
                        }
                    }
                    if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                        if let Type::Reference(type_reference) = &mut **ty {
                            type_reference.lifetime = None;
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}