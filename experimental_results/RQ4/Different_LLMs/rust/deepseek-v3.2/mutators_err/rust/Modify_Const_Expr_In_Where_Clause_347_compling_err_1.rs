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

pub struct Modify_Const_Expr_In_Where_Clause_347;

impl Mutator for Modify_Const_Expr_In_Where_Clause_347 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Where_Clause_347"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_const_param = false;
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_param = true;
                        break;
                    }
                }
                if !has_const_param {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    let mut visitor = ConstExprVisitor::new(&mut rng);
                                    visitor.visit_type_mut(&mut pred_type.bounded_ty);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks with const generic parameters, specifically modifying constant expressions within where clause bounds. It identifies where clauses that contain array types with constant expressions referencing generic const parameters (e.g., `[(); N + 1]`) and mutates the arithmetic operation or constant value (e.g., changing `N + 1` to `N + 2`, `N * 2`, or `N - 1`). This transformation aims to stress the compiler's const evaluation during trait selection and type checking, potentially triggering missing constant errors or evaluation path divergences."
    }
}

struct ConstExprVisitor<'a, R: Rng> {
    rng: &'a mut R,
}

impl<'a, R: Rng> ConstExprVisitor<'a, R> {
    fn new(rng: &'a mut R) -> Self {
        Self { rng }
    }
}

impl<'a, R: Rng> VisitMut for ConstExprVisitor<'a, R> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Binary(expr_bin) = expr {
            if let (Expr::Path(left_path), Expr::Lit(right_lit)) = (&*expr_bin.left, &*expr_bin.right) {
                if let Some(last_seg) = left_path.path.segments.last() {
                    if last_seg.ident.to_string().chars().next().map_or(false, |c| c.is_uppercase()) {
                        let ops = vec![BinOp::Add(Default::default()), BinOp::Mul(Default::default()), BinOp::Sub(Default::default())];
                        expr_bin.op = ops.choose(self.rng).unwrap().clone();
                        if let Lit::Int(lit_int) = &right_lit.lit {
                            let current_val = lit_int.base10_parse::<usize>().unwrap_or(1);
                            let new_val = match expr_bin.op {
                                BinOp::Add(_) => current_val + 1,
                                BinOp::Mul(_) => current_val * 2,
                                BinOp::Sub(_) => current_val.saturating_sub(1),
                                _ => current_val,
                            };
                            expr_bin.right = Box::new(Expr::Lit(Lit::new_int(new_val as u64, lit_int.suffix().into())));
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}