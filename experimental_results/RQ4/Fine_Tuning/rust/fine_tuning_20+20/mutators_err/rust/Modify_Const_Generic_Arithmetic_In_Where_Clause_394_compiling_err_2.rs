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

pub struct Modify_Const_Generic_Arithmetic_In_Where_Clause_394;

impl Mutator for Modify_Const_Generic_Arithmetic_In_Where_Clause_394 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Arithmetic_In_Where_Clause_394"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.const_params().next() {
                    let mut has_usize = false;
                    let mut has_i64 = false;
                    for param in &item_struct.generics.params {
                        if let syn::GenericParam::Const(const_param) = param {
                            if const_param.ty == parse_quote!(usize) {
                                has_usize = true;
                            } else if const_param.ty == parse_quote!(i64) {
                                has_i64 = true;
                            }
                        }
                    }
                    if has_usize && has_i64 {
                        let new_generics = item_struct
                            .generics
                            .params
                            .iter()
                            .map(|param| match param {
                                syn::GenericParam::Const(const_param) => {
                                    if const_param.ty == parse_quote!(i64) {
                                        syn::GenericParam::Const(syn::ConstParam {
                                            ty: parse_quote!(usize),
                                            ..const_param.clone()
                                        })
                                    } else {
                                        param.clone()
                                    }
                                }
                                _ => param.clone(),
                            })
                            .collect::<Punctuated<_, Comma>>();
                        item_struct.generics.params = new_generics;
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                if let Some(generics) = item_impl.generics.const_params().next() {
                    let mut has_usize = false;
                    let mut has_i64 = false;
                    for param in &item_impl.generics.params {
                        if let syn::GenericParam::Const(const_param) = param {
                            if const_param.ty == parse_quote!(usize) {
                                has_usize = true;
                            } else if const_param.ty == parse_quote!(i64) {
                                has_i64 = true;
                            }
                        }
                    }
                    if has_usize && has_i64 {
                        let new_generics = item_impl
                            .generics
                            .params
                            .iter()
                            .map(|param| match param {
                                syn::GenericParam::Const(const_param) => {
                                    if const_param.ty == parse_quote!(i64) {
                                        syn::GenericParam::Const(syn::ConstParam {
                                            ty: parse_quote!(usize),
                                            ..const_param.clone()
                                        })
                                    } else {
                                        param.clone()
                                    }
                                }
                                _ => param.clone(),
                            })
                            .collect::<Punctuated<_, Comma>>();
                        item_impl.generics.params = new_generics;
                    }
                }
                if let Some((_, ref mut path, _)) = item_impl.trait_ {
                    if path.segments.len() == 1 {
                        let segment = path.segments.first().unwrap();
                        if segment.ident == "Container" {
                            let new_path = parse_quote!(Container);
                            *path = new_path;
                        }
                    }
                }
                if let Some((_, ref mut path, _)) = item_impl.trait_ {
                    if path.segments.len() == 1 {
                        let segment = path.segments.first().unwrap();
                        if segment.ident == "Container" {
                            let new_path = parse_quote!(Container);
                            *path = new_path;
                        }
                    }
                }
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Array(type_array) = &predicate_type.bounded_ty {
                                if let syn::Expr::Cast(expr_cast) = &*type_array.len {
                                    if let syn::Expr::Binary(expr_binary) = &*expr_cast.expr {
                                        let new_expr = parse_quote!(X + (Y * 2));
                                        let new_cast = parse_quote!(#new_expr as usize);
                                        *predicate = parse_quote!([(); #new_cast]:);
                                    }
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "new" {
                            let dummy_expr: syn::Expr = parse_quote!(X + (Y * 2) as i64);
                            let dummy_stmt = syn::Stmt::Local(syn::Local {
                                attrs: Vec::new(),
                                let_token: Default::default(),
                                pat: syn::Pat::Wild(syn::PatWild {
                                    attrs: Vec::new(),
                                    underscore_token: Default::default(),
                                }),
                                init: Some(syn::LocalInit {
                                    eq_token: Default::default(),
                                    expr: Box::new(dummy_expr),
                                    diverge: None,
                                }),
                                semi_token: Default::default(),
                            });
                            method.block.stmts.insert(0, dummy_stmt);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs and their implementations that use constant generics with mixed types (e.g., usize and i64). It modifies the struct and impl to use the same type for all const parameters (usize) and alters arithmetic operations in the where clause to use mixed constant types. Additionally, it injects a dummy arithmetic operation involving the constants in the impl, forcing evaluation and increasing the likelihood of triggering type checker errors."
    }
}