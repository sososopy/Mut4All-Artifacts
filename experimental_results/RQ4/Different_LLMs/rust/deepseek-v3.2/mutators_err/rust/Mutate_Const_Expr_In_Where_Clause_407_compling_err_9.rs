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

pub struct Mutate_Const_Expr_In_Where_Clause_407;

impl Mutator for Mutate_Const_Expr_In_Where_Clause_407 {
    fn name(&self) -> &str {
        "Mutate_Const_Expr_In_Where_Clause_407"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let has_const_param = item_impl.generics.params.iter().any(|param| {
                    if let GenericParam::Const(_) = param {
                        true
                    } else {
                        false
                    }
                });
                if !has_const_param {
                    continue;
                }
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred_type) = predicate {
                            if let Type::Slice(type_slice) = &mut pred_type.bounded_ty {
                                if let Type::Array(type_array) = &mut *type_slice.elem {
                                    self.mutate_const_expr(&mut type_array.len, &item_impl.generics.params, &mut rng);
                                }
                            }
                            if let Type::Array(type_array) = &mut pred_type.bounded_ty {
                                self.mutate_const_expr(&mut type_array.len, &item_impl.generics.params, &mut rng);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks with generic const parameters and where clauses containing const expressions (e.g., array length expressions). It mutates the const expression within the where clause by altering arithmetic operators or numeric literals, using only existing const parameters or simple integer literals. This transformation stresses the compiler's const evaluation and trait resolution for complex const generic interactions, potentially triggering bugs in const expression validation or where clause satisfaction."
    }
}

impl Mutate_Const_Expr_In_Where_Clause_407 {
    fn mutate_const_expr(&self, expr: &mut Expr, const_params: &Punctuated<GenericParam, Comma>, rng: &mut impl Rng) {
        match expr {
            Expr::Binary(expr_bin) => {
                let ops: [syn::BinOp; 4] = [
                    syn::BinOp::Add(token::Plus::default()),
                    syn::BinOp::Sub(token::Minus::default()),
                    syn::BinOp::Mul(token::Star::default()),
                    syn::BinOp::Div(token::Slash::default()),
                ];
                let new_op = ops.choose(rng).unwrap();
                expr_bin.op = new_op.clone();
                self.mutate_const_expr(&mut expr_bin.left, const_params, rng);
                self.mutate_const_expr(&mut expr_bin.right, const_params, rng);
            }
            Expr::Lit(expr_lit) => {
                if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                    let new_val = match lit_int.base10_parse::<u64>() {
                        Ok(val) => {
                            let choices = [0u64, 1, 2, val.wrapping_add(1), val.wrapping_sub(1)];
                            *choices.choose(rng).unwrap()
                        }
                        Err(_) => 0,
                    };
                    *lit_int = syn::LitInt::new(&new_val.to_string(), lit_int.span());
                }
            }
            Expr::Path(expr_path) => {
                let const_param_vec: Vec<_> = const_params.iter().filter_map(|p| {
                    if let GenericParam::Const(const_param) = p {
                        Some(const_param)
                    } else {
                        None
                    }
                }).collect();
                if let Some(const_param) = const_param_vec.choose(rng) {
                    expr_path.path = const_param.ident.clone().into();
                }
            }
            _ => {
                let const_param_names: Vec<Ident> = const_params.iter().filter_map(|p| {
                    if let GenericParam::Const(const_param) = p {
                        Some(const_param.ident.clone())
                    } else {
                        None
                    }
                }).collect();
                if !const_param_names.is_empty() {
                    let new_expr: Expr = if rng.gen_bool(0.5) {
                        let param = const_param_names.choose(rng).unwrap();
                        parse_quote!(#param)
                    } else {
                        let literal = rng.gen_range(0..=5);
                        parse_quote!(#literal)
                    };
                    *expr = new_expr;
                }
            }
        }
    }
}