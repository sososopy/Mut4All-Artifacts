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

pub struct Alter_Const_Generic_Expression_356;

impl Mutator for Alter_Const_Generic_Expression_356 {
    fn name(&self) -> &str {
        "Alter_Const_Generic_Expression_356"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(expr) = &mut const_param.default {
                            mutate_const_expr(&mut rng, expr);
                        }
                    }
                }
                if let Some(where_clause) = &mut item_struct.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_predicate) = predicate {
                            if let syn::Type::Array(type_array) = &type_predicate.bounded_ty {
                                if let Some(expr) = &mut type_array.len {
                                    mutate_const_expr(&mut rng, expr);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Fn(item_fn) = item {
                for param in &mut item_fn.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(expr) = &mut const_param.default {
                            mutate_const_expr(&mut rng, expr);
                        }
                    }
                }
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_predicate) = predicate {
                            if let syn::Type::Array(type_array) = &type_predicate.bounded_ty {
                                if let Some(expr) = &mut type_array.len {
                                    mutate_const_expr(&mut rng, expr);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Const(item_const) = impl_item {
                        if let syn::Expr::Binary(expr_bin) = &mut item_const.expr {
                            mutate_const_expr(&mut rng, expr_bin);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies constant generic expressions in struct definitions, function signatures, and associated constants. It targets expressions involving arithmetic operations or comparisons, swapping operators, modifying operands, or introducing parentheses to alter evaluation order. This transformation stresses the compiler's const evaluation system, potentially causing type constraint violations, array length mismatches, or const generic bound failures."
    }
}

fn mutate_const_expr(rng: &mut impl Rng, expr: &mut syn::Expr) {
    if let syn::Expr::Binary(expr_bin) = expr {
        let operators = [
            syn::BinOp::Add(token::Plus::default()),
            syn::BinOp::Sub(token::Minus::default()),
            syn::BinOp::Mul(token::Star::default()),
            syn::BinOp::Div(token::Slash::default()),
        ];
        expr_bin.op = *operators.choose(rng).unwrap();
        if rng.gen_bool(0.5) {
            let left = expr_bin.left.clone();
            let right = expr_bin.right.clone();
            expr_bin.left = Box::new(syn::Expr::Paren(syn::ExprParen {
                attrs: Vec::new(),
                paren_token: token::Paren::default(),
                expr: left,
            }));
            expr_bin.right = Box::new(syn::Expr::Paren(syn::ExprParen {
                attrs: Vec::new(),
                paren_token: token2::Paren::default(),
                expr: right,
            }));
        }
    }
}