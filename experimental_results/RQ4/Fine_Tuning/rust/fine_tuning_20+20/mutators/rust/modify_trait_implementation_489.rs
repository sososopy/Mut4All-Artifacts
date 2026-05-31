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

pub struct Modify_Trait_Implementation_489;

impl Mutator for Modify_Trait_Implementation_489 {
    fn name(&self) -> &str {
        "Modify_Trait_Implementation_489"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut used_params = HashSet::new();
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for segment in &path.segments {
                        if let PathArguments::AngleBracketed(angle_bracketed) =
                            &segment.arguments
                        {
                            for arg in &angle_bracketed.args {
                                if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                    for segment in &type_path.path.segments {
                                        used_params.insert(segment.ident.to_string());
                                    }
                                }
                            }
                        }
                    }
                }
                for item in &item_impl.items {
                    match item {
                        ImplItem::Const(item_const) => {
                            if let Type::Path(type_path) = &item_const.ty {
                                for segment in &type_path.path.segments {
                                    used_params.insert(segment.ident.to_string());
                                }
                            }
                        }
                        ImplItem::Fn(item_method) => {
                            for input in &item_method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Path(type_path) = &*pat_type.ty {
                                        for segment in &type_path.path.segments {
                                            used_params.insert(segment.ident.to_string());
                                        }
                                    }
                                }
                            }
                            if let ReturnType::Type(_, ty) = &item_method.sig.output {
                                if let Type::Path(type_path) = &**ty {
                                    for segment in &type_path.path.segments {
                                        used_params.insert(segment.ident.to_string());
                                    }
                                }
                            }
                        }
                        ImplItem::Type(item_type) => {
                            if let Type::Path(type_path) = &item_type.ty {
                                for segment in &type_path.path.segments {
                                    used_params.insert(segment.ident.to_string());
                                }
                            }
                        }
                        _ => {}
                    }
                }
                let mut new_generics = item_impl.generics.clone();
                new_generics.params = new_generics
                    .params
                    .into_iter()
                    .filter(|param| match param {
                        GenericParam::Type(type_param) => {
                            used_params.contains(&type_param.ident.to_string())
                        }
                        GenericParam::Lifetime(lifetime_param) => {
                            used_params.contains(&lifetime_param.lifetime.ident.to_string())
                        }
                        GenericParam::Const(const_param) => {
                            used_params.contains(&const_param.ident.to_string())
                        }
                    })
                    .collect();
                item_impl.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets trait implementations by removing unused type parameters from the `impl` block. It analyzes the usage of type, lifetime, and const parameters within the trait path, associated types, methods, and constants. Unused parameters are pruned from the implementation's generics, streamlining the code and potentially exposing compiler weaknesses in handling trait bounds and type inference."
    }
}