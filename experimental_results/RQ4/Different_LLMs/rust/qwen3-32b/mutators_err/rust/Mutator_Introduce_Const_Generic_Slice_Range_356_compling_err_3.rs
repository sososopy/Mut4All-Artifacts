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

pub struct Mutator_Introduce_Const_Generic_Slice_Range_356;

impl Mutator for Mutator_Introduce_Const_Generic_Slice_Range_356 {
    fn name(&self) -> &str {
        "Mutator_Introduce_Const_Generic_Slice_Range_356"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let const_params: Vec<_> = func.sig.generics.params.iter()
                    .filter_map(|param| {
                        if let syn::GenericParam::Const(param) = param {
                            Some(param)
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.is_empty() {
                    continue;
                }
                let first_const_name = const_params[0].ident.clone();
                let block = &mut func.block;
                for stmt in &mut block.stmts {
                    if let syn::Stmt::Expr(expr, _) = stmt {
                        if let syn::Expr::Call(call_expr) = &mut *expr {
                            if let syn::Expr::Path(_) = &*call_expr.func {
                                for arg in &mut call_expr.args {
                                    if let syn::Expr::Index(index_expr) = &mut *arg {
                                        if let syn::Expr::Range(range_expr) = &*index_expr.index {
                                            if range_expr.end.is_none() {
                                                let new_end: Expr = parse_quote!(#first_const_name);
                                                let new_range: Expr = parse_quote!(..#new_end);
                                                index_expr.index = Box::new(syn::Expr::Range(new_range));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let const_params: Vec<_> = method.sig.generics.params.iter()
                            .filter_map(|param| {
                                if let syn::GenericParam::Const(param) = param {
                                    Some(param)
                                } else {
                                    None
                                }
                            })
                            .collect();
                        if const_params.is_empty() {
                            continue;
                        }
                        let first_const_name = const_params[0].ident.clone();
                        let block = &mut method.block;
                        for stmt in &mut block.stmts {
                            if let syn::Stmt::Expr(expr, _) = stmt {
                                if let syn::Expr::Call(call_expr) = &mut *expr {
                                    if let syn::Expr::Path(_) = &*call_expr.func {
                                        for arg in &mut call_expr.args {
                                            if let syn::Expr::Index(index_expr) = &mut *arg {
                                                if let syn::Expr::Range(range_expr) = &*index_expr.index {
                                                    if range_expr.end.is_none() {
                                                        let new_end: Expr = parse_quote!(#first_const_name);
                                                        let new_range: Expr = parse_quote!(..#new_end);
                                                        index_expr.index = Box::new(syn::Expr::Range(new_range));
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}