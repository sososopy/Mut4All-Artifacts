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

pub struct Swap_Binary_Operator_Or_Operands_56;

impl Mutator for Swap_Binary_Operator_Or_Operands_56 {
    fn name(&self) -> &str {
        "Swap_Binary_Operator_Or_Operands_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = BinaryExprVisitor {
            rng: &mut rng,
            changed: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets binary expressions where operands have different integer types, especially those involving unsafe union field accesses. It either swaps the operator to another from the set {Add, Sub, Mul, Div, Rem, BitAnd, BitOr, BitXor, Shl, Shr, Eq, Ne, Lt, Le, Gt, Ge} or swaps operand order for non-commutative operators. This transformation aims to trigger different type coercion or overflow behaviors, testing the compiler's handling of type mismatches and asymmetric operations during const-evaluation or MIR transformation."
    }
}

struct BinaryExprVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    changed: bool,
}

impl<'a> VisitMut for BinaryExprVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Binary(binary_expr) = expr {
            let left_type = infer_type(&binary_expr.left);
            let right_type = infer_type(&binary_expr.right);
            if left_type != right_type && (is_integer_type(&left_type) && is_integer_type(&right_type)) {
                let non_commutative_ops = vec![
                    syn::BinOp::Sub(token::Minus),
                    syn::BinOp::Div(token::Slash),
                    syn::BinOp::Rem(token::Percent),
                    syn::BinOp::Shl(token::Shl),
                    syn::BinOp::Shr(token::Shr),
                    syn::BinOp::Lt(token::Lt),
                    syn::BinOp::Le(token::Le),
                    syn::BinOp::Gt(token::Gt),
                    syn::BinOp::Ge(token::Ge),
                ];
                let all_ops = vec![
                    syn::BinOp::Add(token::Plus),
                    syn::BinOp::Sub(token::Minus),
                    syn::BinOp::Mul(token::Star),
                    syn::BinOp::Div(token::Slash),
                    syn::BinOp::Rem(token::Percent),
                    syn::BinOp::BitAnd(token::And),
                    syn::BinOp::BitOr(token::Or),
                    syn::BinOp::BitXor(token::Caret),
                    syn::BinOp::Shl(token::Shl),
                    syn::BinOp::Shr(token::Shr),
                    syn::BinOp::Eq(token::Eq),
                    syn::BinOp::Ne(token::Ne),
                    syn::BinOp::Lt(token::Lt),
                    syn::BinOp::Le(token::Le),
                    syn::BinOp::Gt(token::Gt),
                    syn::BinOp::Ge(token::Ge),
                ];
                if self.rng.gen_bool(0.5) && non_commutative_ops.contains(&binary_expr.op) {
                    let mut left = binary_expr.left.clone();
                    let mut right = binary_expr.right.clone();
                    binary_expr.left = right;
                    binary_expr.right = left;
                    self.changed = true;
                } else {
                    let mut new_op = binary_expr.op;
                    while new_op == binary_expr.op {
                        new_op = *all_ops.choose(self.rng).unwrap();
                    }
                    binary_expr.op = new_op;
                    self.changed = true;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn infer_type(expr: &syn::Expr) -> String {
    match expr {
        syn::Expr::Path(path_expr) => {
            if let Some(segment) = path_expr.path.segments.last() {
                segment.ident.to_string()
            } else {
                "unknown".to_string()
            }
        },
        syn::Expr::Lit(lit_expr) => {
            match &lit_expr.lit {
                syn::Lit::Int(int_lit) => int_lit.suffix().to_string(),
                syn::Lit::Float(float_lit) => float_lit.suffix().to_string(),
                _ => "unknown".to_string(),
            }
        },
        syn::Expr::Cast(cast_expr) => {
            match &*cast_expr.ty {
                syn::Type::Path(type_path) => {
                    if let Some(segment) = type_path.path.segments.last() {
                        segment.ident.to_string()
                    } else {
                        "unknown".to_string()
                    }
                },
                _ => "unknown".to_string(),
            }
        },
        syn::Expr::Unsafe(unsafe_expr) => {
            if let syn::Expr::Block(block_expr) = &unsafe_expr.block {
                if let Some(stmt) = block_expr.block.stmts.last() {
                    if let syn::Stmt::Expr(expr_stmt, _) = stmt {
                        infer_type(&expr_stmt)
                    } else {
                        "unknown".to_string()
                    }
                } else {
                    "unknown".to_string()
                }
            } else {
                "unknown".to_string()
            }
        },
        syn::Expr::Field(field_expr) => {
            infer_type(&field_expr.base)
        },
        _ => "unknown".to_string(),
    }
}

fn is_integer_type(type_str: &str) -> bool {
    type_str.starts_with("i") || type_str.starts_with("u") || type_str == "isize" || type_str == "usize"
}