use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Generic_Parameter_With_Another_Generic_Parameter_278;

impl Mutator for Replace_Generic_Parameter_With_Another_Generic_Parameter_278 {
    fn name(&self) -> &str {
        "Replace_Generic_Parameter_With_Another_Generic_Parameter_278"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    let params: Vec<_> = generics.params.iter().collect();
                    if !params.is_empty() {
                        let param_to_replace = params.choose(&mut thread_rng()).unwrap();
                        let replacement_param = params.iter().find(|p| p != &param_to_replace).unwrap();
                        replace_generic_parameter(func, param_to_replace, replacement_param);
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    let params: Vec<_> = generics.params.iter().collect();
                    if !params.is_empty() {
                        let param_to_replace = params.choose(&mut thread_rng()).unwrap();
                        let replacement_param = params.iter().find(|p| p != &param_to_replace).unwrap();
                        replace_generic_parameter_in_impl(impl_item, param_to_replace, replacement_param);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces one generic parameter with another in function and method signatures. This transformation tests the compiler's handling of generic parameter substitutions and may lead to type-related errors or inconsistencies."
    }
}

fn replace_generic_parameter(func: &mut syn::ItemFn, param_to_replace: &syn::GenericParam, replacement_param: &syn::GenericParam) {
    for arg in &mut func.sig.inputs {
        if let FnArg::Typed(pat_type) = arg {
            if let Type::Path(type_path) = &*pat_type.ty {
                if let Some(segment) = type_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    match param_to_replace {
                                        syn::GenericParam::Type(param) => {
                                            if segment.ident == param.ident {
                                                segment.ident = match replacement_param {
                                                    syn::GenericParam::Type(replacement) => replacement.ident.clone(),
                                                    _ => panic!("Cannot replace type parameter with non-type parameter"),
                                                };
                                            }
                                        }
                                        syn::GenericParam::Lifetime(param) => {
                                            if let syn::GenericParam::Lifetime(replacement) = replacement_param {
                                                if param.lifetime == replacement.lifetime {
                                                    // Do nothing, cannot replace Ident with Lifetime
                                                }
                                            }
                                        }
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for stmt in &mut func.block.stmts {
        if let Stmt::Expr(expr, _) = stmt {
            if let Expr::Path(expr_path) = expr {
                if let Some(segment) = expr_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    match param_to_replace {
                                        syn::GenericParam::Type(param) => {
                                            if segment.ident == param.ident {
                                                segment.ident = match replacement_param {
                                                    syn::GenericParam::Type(replacement) => replacement.ident.clone(),
                                                    _ => panic!("Cannot replace type parameter with non-type parameter"),
                                                };
                                            }
                                        }
                                        syn::GenericParam::Lifetime(param) => {
                                            if let syn::GenericParam::Lifetime(replacement) = replacement_param {
                                                if param.lifetime == replacement.lifetime {
                                                    // Do nothing, cannot replace Ident with Lifetime
                                                }
                                            }
                                        }
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

fn replace_generic_parameter_in_impl(impl_item: &mut syn::ItemImpl, param_to_replace: &syn::GenericParam, replacement_param: &syn::GenericParam) {
    for impl_item in &mut impl_item.items {
        if let syn::ImplItem::Fn(func) = impl_item {
            replace_generic_parameter_in_impl_item_fn(func, param_to_replace, replacement_param);
        }
    }
}

fn replace_generic_parameter_in_impl_item_fn(func: &mut syn::ImplItemFn, param_to_replace: &syn::GenericParam, replacement_param: &syn::GenericParam) {
    for arg in &mut func.sig.inputs {
        if let FnArg::Typed(pat_type) = arg {
            if let Type::Path(type_path) = &*pat_type.ty {
                if let Some(segment) = type_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    match param_to_replace {
                                        syn::GenericParam::Type(param) => {
                                            if segment.ident == param.ident {
                                                segment.ident = match replacement_param {
                                                    syn::GenericParam::Type(replacement) => replacement.ident.clone(),
                                                    _ => panic!("Cannot replace type parameter with non-type parameter"),
                                                };
                                            }
                                        }
                                        syn::GenericParam::Lifetime(param) => {
                                            if let syn::GenericParam::Lifetime(replacement) = replacement_param {
                                                if param.lifetime == replacement.lifetime {
                                                    // Do nothing, cannot replace Ident with Lifetime
                                                }
                                            }
                                        }
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for stmt in &mut func.block.stmts {
        if let Stmt::Expr(expr, _) = stmt {
            if let Expr::Path(expr_path) = expr {
                if let Some(segment) = expr_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    match param_to_replace {
                                        syn::GenericParam::Type(param) => {
                                            if segment.ident == param.ident {
                                                segment.ident = match replacement_param {
                                                    syn::GenericParam::Type(replacement) => replacement.ident.clone(),
                                                    _ => panic!("Cannot replace type parameter with non-type parameter"),
                                                };
                                            }
                                        }
                                        syn::GenericParam::Lifetime(param) => {
                                            if let syn::GenericParam::Lifetime(replacement) = replacement_param {
                                                if param.lifetime == replacement.lifetime {
                                                    // Do nothing, cannot replace Ident with Lifetime
                                                }
                                            }
                                        }
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}