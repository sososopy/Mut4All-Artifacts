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

pub struct Swap_Trait_References_146;

impl Mutator for Swap_Trait_References_146 {
    fn name(&self) -> &str {
        "Swap_Trait_References_146"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut traits: Vec<syn::Path> = Vec::new();
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                traits.push(trait_item.ident.clone().into());
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                let mut trait_references: Vec<(usize, syn::Type)> = Vec::new();
                for (index, input) in func.sig.inputs.iter().enumerate() {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.len() == 1 {
                                trait_references.push((index, pat_type.ty.clone()));
                            }
                        }
                    }
                }

                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(type_path) = return_type.as_ref() {
                        if type_path.path.segments.len() == 1 {
                            trait_references.push((func.sig.inputs.len(), return_type.clone()));
                        }
                    }
                }

                if trait_references.len() >= 2 {
                    let mut rng = thread_rng();
                    let index1 = rng.gen_range(0..trait_references.len());
                    let index2 = rng.gen_range(0..trait_references.len());
                    let (index1, _) = trait_references[index1];
                    let (index2, _) = trait_references[index2];

                    if index1 < func.sig.inputs.len() {
                        if let FnArg::Typed(pat_type) = &mut func.sig.inputs[index1] {
                            if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index2) {
                                *pat_type.ty = ty.clone();
                            }
                        }
                    } else {
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index2) {
                                *return_type = ty.clone();
                            }
                        }
                    }

                    if index2 < func.sig.inputs.len() {
                        if let FnArg::Typed(pat_type) = &mut func.sig.inputs[index2] {
                            if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index1) {
                                *pat_type.ty = ty.clone();
                            }
                        }
                    } else {
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index1) {
                                *return_type = ty.clone();
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut trait_references: Vec<(usize, syn::Type)> = Vec::new();
                        for (index, input) in func.sig.inputs.iter().enumerate() {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if type_path.path.segments.len() == 1 {
                                        trait_references.push((index, pat_type.ty.clone()));
                                    }
                                }
                            }
                        }

                        if let ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::Path(type_path) = return_type.as_ref() {
                                if type_path.path.segments.len() == 1 {
                                    trait_references.push((func.sig.inputs.len(), return_type.clone()));
                                }
                            }
                        }

                        if trait_references.len() >= 2 {
                            let mut rng = thread_rng();
                            let index1 = rng.gen_range(0..trait_references.len());
                            let index2 = rng.gen_range(0..trait_references.len());
                            let (index1, _) = trait_references[index1];
                            let (index2, _) = trait_references[index2];

                            if index1 < func.sig.inputs.len() {
                                if let FnArg::Typed(pat_type) = &mut func.sig.inputs[index1] {
                                    if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index2) {
                                        *pat_type.ty = ty.clone();
                                    }
                                }
                            } else {
                                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                                    if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index2) {
                                        *return_type = ty.clone();
                                    }
                                }
                            }

                            if index2 < func.sig.inputs.len() {
                                if let FnArg::Typed(pat_type) = &mut func.sig.inputs[index2] {
                                    if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index1) {
                                        *pat_type.ty = ty.clone();
                                    }
                                }
                            } else {
                                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                                    if let Some((_, ty)) = trait_references.iter().find(|(i, _)| *i == index1) {
                                        *return_type = ty.clone();
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
        "The mutation operator Swap_Trait_References_146 swaps the trait references in function and method signatures. It randomly selects two trait references and swaps them, potentially triggering bugs related to trait object safety, cyclic dependencies, and incorrect trait usage."
    }
}