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

pub struct Modify_Const_Fn_To_Inline_Closure_75;

impl Mutator for Modify_Const_Fn_To_Inline_Closure_75 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_To_Inline_Closure_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_fn_idents = HashSet::new();

        for item in &file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Type::BareFn(bare_fn) = &*item_const.ty {
                    if bare_fn.lifetimes.is_none() && bare_fn.inputs.is_empty() {
                        if let syn::Expr::Closure(closure_expr) = &*item_const.expr {
                            const_fn_idents.insert(item_const.ident.clone());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                item_fn.block.stmts.iter_mut().for_each(|stmt| {
                    if let syn::Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let syn::Expr::Reference(expr_ref) = &**expr {
                                if let syn::Expr::Path(expr_path) = &*expr_ref.expr {
                                    if let Some(ident) = expr_path.path.get_ident() {
                                        if const_fn_idents.contains(ident) {
                                            if let syn::Item::Const(item_const) = file.items.iter().find(|item| {
                                                if let syn::Item::Const(item_const) = item {
                                                    item_const.ident == *ident
                                                } else {
                                                    false
                                                }
                                            }).unwrap() {
                                                if let syn::Expr::Closure(closure_expr) = &*item_const.expr {
                                                    local.init = Some((local.init.as_ref().unwrap().0.clone(), Box::new(syn::Expr::Closure(closure_expr.clone()))));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies const functions that return closures and inlines those closures directly at their usage points, removing the const function declaration. The transformation simplifies the code by eliminating indirection, potentially revealing optimization or inlining issues within the compiler."
    }
}