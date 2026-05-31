use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAssign, ExprBinary, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Subtraction_With_Addition_96;

impl Mutator for Replace_Subtraction_With_Addition_96 {
    fn name(&self) -> &str {
        "Replace_Subtraction_With_Addition_96"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let const_params: HashSet<Ident> = func.sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(param) = param {
                            Some(param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if let Some(block) = func.block.as_mut() {
                    for stmt in &mut block.stmts {
                        match stmt {
                            Stmt::Expr(expr, _) => {
                                if let Expr::Assign(assign_expr) = expr {
                                    if let Expr::Binary(ref mut binary_expr) = *assign_expr.right {
                                        if let BinOp::Sub(_) = binary_expr.op {
                                            if let Expr::Path(ref mut expr_path) = *binary_expr.left {
                                                if let Some(segment) = expr_path.path.segments.last() {
                                                    if const_params.contains(&segment.ident) {
                                                        binary_expr.op = BinOp::Add(Default::default());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            },
                            Stmt::Local(local) => {
                                if let Some(LocalInit { expr, .. }) = &mut local.init {
                                    if let Expr::Binary(ref mut binary_expr) = *expr {
                                        if let BinOp::Sub(_) = binary_expr.op {
                                            if let Expr::Path(ref mut expr_path) = *binary_expr.left {
                                                if let Some(segment) = expr_path.path.segments.last() {
                                                    if const_params.contains(&segment.ident) {
                                                        binary_expr.op = BinOp::Add(Default::default());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            },
                            _ => {}
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let const_params: HashSet<Ident> = method.sig
                            .generics
                            .params
                            .iter()
                            .filter_map(|param| {
                                if let GenericParam::Const(param) = param {
                                    Some(param.ident.clone())
                                } else {
                                    None
                                }
                            })
                            .collect();
                        if let Some(block) = method.block.as_mut() {
                            for stmt in &mut block.stmts {
                                match stmt {
                                    Stmt::Expr(expr, _) => {
                                        if let Expr::Assign(assign_expr) = expr {
                                            if let Expr::Binary(ref mut binary_expr) = *assign_expr.right {
                                                if let BinOp::Sub(_) = binary_expr.op {
                                                    if let Expr::Path(ref mut expr_path) = *binary_expr.left {
                                                        if let Some(segment) = expr_path.path.segments.last() {
                                                            if const_params.contains(&segment.ident) {
                                                                binary_expr.op = BinOp::Add(Default::default());
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    },
                                    Stmt::Local(local) => {
                                        if let Some(LocalInit { expr, .. }) = &mut local.init {
                                            if let Expr::Binary(ref mut binary_expr) = *expr {
                                                if let BinOp::Sub(_) = binary_expr.op {
                                                    if let Expr::Path(ref mut expr_path) = *binary_expr.left {
                                                        if let Some(segment) = expr_path.path.segments.last() {
                                                            if const_params.contains(&segment.ident) {
                                                                binary_expr.op = BinOp::Add(Default::default());
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    },
                                    _ => {}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces subtraction operations in const parameter-based index calculations with addition, potentially causing out-of-bounds indices. This transformation targets assignments where a const generic is used in arithmetic to compute an index or length, altering the operator to provoke array bounds violations or ICEs during const evaluation."
    }
}