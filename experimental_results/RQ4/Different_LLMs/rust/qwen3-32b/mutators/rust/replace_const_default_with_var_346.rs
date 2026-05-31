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

pub struct Replace_Const_Default_With_Var_346;

impl Mutator for Replace_Const_Default_With_Var_346 {
    fn name(&self) -> &str {
        "Replace_Const_Default_With_Var_346"
    }
    fn mutate(&self, file: &mut syn::File) {
        let non_const_vars: Vec<Ident> = file.items.iter()
            .filter_map(|item| {
                if let Item::Fn(item_fn) = item {
                    Some(item_fn.block.stmts.iter()
                        .filter_map(|stmt| {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Ident(pat_ident) = &local.pat {
                                    Some(pat_ident.ident.clone())
                                } else {
                                    None
                                }
                            } else {
                                None
                            }
                        })
                        .collect())
                } else if let Item::Impl(item_impl) = item {
                    let mut vars = Vec::new();
                    for impl_item in &item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            vars.extend(method.block.stmts.iter()
                                .filter_map(|stmt| {
                                    if let Stmt::Local(local) = stmt {
                                        if let Pat::Ident(pat_ident) = &local.pat {
                                            Some(pat_ident.ident.clone())
                                        } else {
                                            None
                                        }
                                    } else {
                                        None
                                    }
                                })
                                .collect::<Vec<_>>());
                        }
                    }
                    if !vars.is_empty() {
                        Some(vars)
                    } else {
                        None
                    }
                } else {
                    None
                }
            })
            .flatten()
            .collect();

        if non_const_vars.is_empty() {
            return;
        }

        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            let var_name = non_const_vars.choose(&mut rng).unwrap().clone();
                            let new_expr = parse_quote! { #var_name };
                            *default_expr = new_expr;
                        }
                    }
                }
            }

            if let Item::Fn(item_fn) = item {
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            let var_name = non_const_vars.choose(&mut rng).unwrap().clone();
                            let new_expr = parse_quote! { #var_name };
                            *default_expr = new_expr;
                        }
                    }
                }
            }

            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for param in &mut method.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Some(default_expr) = &mut const_param.default {
                                    let var_name = non_const_vars.choose(&mut rng).unwrap().clone();
                                    let new_expr = parse_quote! { #var_name };
                                    *default_expr = new_expr;
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