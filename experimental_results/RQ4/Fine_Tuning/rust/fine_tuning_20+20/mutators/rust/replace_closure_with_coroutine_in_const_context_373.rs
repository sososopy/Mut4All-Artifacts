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

pub struct Replace_Closure_With_Coroutine_In_Const_Context_373;

impl Mutator for Replace_Closure_With_Coroutine_In_Const_Context_373 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Coroutine_In_Const_Context_373"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureReplacer;
        impl VisitMut for ClosureReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(_) = expr {
                    *expr = parse_quote! {
                        {
                            let __f__ = async {};
                            __f__.await
                        }
                    };
                } else {
                    syn::visit_mut::visit_expr_mut(self, expr);
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("feature"))
                {
                    for attr in &item_fn.attrs {
                        if attr.path().is_ident("feature") {
                            if attr
                                .meta
                                .require_path_only()
                                .unwrap()
                                .to_token_stream()
                                .to_string()
                                .contains("generic_const_exprs")
                            {
                                if let Some(const_param) = item_fn
                                    .sig
                                    .generics
                                    .params
                                    .iter()
                                    .find_map(|param| {
                                        if let GenericParam::Const(const_param) = param {
                                            Some(const_param)
                                        } else {
                                            None
                                        }
                                    })
                                {
                                    let const_ident = &const_param.ident;
                                    let const_expr = &const_param.ty;
                                    let mut replacer = ClosureReplacer;
                                    let mut mutated_expr = const_expr.clone();
                                    replacer.visit_type_mut(&mut mutated_expr);
                                    let new_const_param: GenericParam = parse_quote! {
                                        const #const_ident: usize = #mutated_expr
                                    };
                                    let mut new_generics = item_fn.sig.generics.clone();
                                    new_generics.params = new_generics
                                        .params
                                        .iter()
                                        .map(|param| {
                                            if let GenericParam::Const(inner_const_param) =
                                                param
                                            {
                                                if inner_const_param.ident == *const_ident {
                                                    new_const_param.clone()
                                                } else {
                                                    param.clone()
                                                }
                                            } else {
                                                param.clone()
                                            }
                                        })
                                        .collect();
                                    item_fn.sig.generics = new_generics;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with the `#![feature(generic_const_exprs)]` attribute and replaces closure expressions within const generic parameters with equivalent coroutine expressions. By transforming closures into coroutines in constant contexts, it stresses the compiler's handling of advanced const evaluation and async features, potentially uncovering bugs related to type inference, const evaluation, and coroutine integration in constant expressions."
    }
}