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

pub struct Introduce_Nested_Arithmetic_Const_Expressions_With_Overlapping_Params_374;

impl Mutator for Introduce_Nested_Arithmetic_Const_Expressions_With_Overlapping_Params_374 {
    fn name(&self) -> &str {
        "Introduce_Nested_Arithmetic_Const_Expressions_With_Overlapping_Params_374"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut const_params = HashSet::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(c) = param {
                        const_params.insert(c.ident.clone());
                    }
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(array) = &mut *pat_type.ty {
                            if let Expr::Path(expr_path) = &**array.len {
                                if expr_path.path.segments.len() == 1 {
                                    let ident = &expr_path.path.segments[0].ident;
                                    if const_params.contains(ident) {
                                        *array.len = Box::new(syn::parse_quote! {{ #ident + 1 - #ident }});
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(type_pred) = predicate {
                            if let Type::Array(array) = &mut *type_pred.bounded_ty {
                                if let Expr::Path(expr_path) = &**array.len {
                                    if expr_path.path.segments.len() == 1 {
                                        let ident = &expr_path.path.segments[0].ident;
                                        if const_params.contains(ident) {
                                            *array.len = Box::new(syn::parse_quote! {{ (#ident - 1) + 1 + #ident }});
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let mut const_params = HashSet::new();
                        for param in &method.sig.generics.params {
                            if let GenericParam::Const(c) = param {
                                const_params.insert(c.ident.clone());
                            }
                        }
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Array(array) = &mut *pat_type.ty {
                                    if let Expr::Path(expr_path) = &**array.len {
                                        if expr_path.path.segments.len() == 1 {
                                            let ident = &expr_path.path.segments[0].ident;
                                            if const_params.contains(ident) {
                                                *array.len = Box::new(syn::parse_quote! {{ #ident + 1 - #ident }});
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(type_pred) = predicate {
                                    if let Type::Array(array) = &mut *type_pred.bounded_ty {
                                        if let Expr::Path(expr_path) = &**array.len {
                                            if expr_path.path.segments.len() == 1 {
                                                let ident = &expr_path.path.segments[0].ident;
                                                if const_params.contains(ident) {
                                                    *array.len = Box::new(syn::parse_quote! {{ (#ident - 1) + 1 + #ident }});
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces nested arithmetic const expressions in generic bounds with overlapping parameters. It targets functions and methods that use const parameters in array types or trait bounds, replacing the const usage with compound expressions combining addition and subtraction of the same parameter. This creates overlapping const evaluations in both array sizes and trait bounds, potentially exposing compiler evaluation bugs or incorrect simplification logic."
    }
}