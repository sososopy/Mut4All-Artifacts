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

pub struct Substitute_Generic_Const_Expressions_With_Different_Arithmetic_Operations_374;

impl Mutator for Substitute_Generic_Const_Expressions_With_Different_Arithmetic_Operations_374 {
    fn name(&self) -> &str {
        "Substitute_Generic_Const_Expressions_With_Different_Arithmetic_Operations_374"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Binary(binary_expr) = expr {
                            let left = &binary_expr.left;
                            let right = &binary_expr.right;
                            let operator = &binary_expr.op;

                            // Substitute with different arithmetic operations
                            let new_operator = match operator {
                                BinOp::Add(_) => BinOp::Mul(token::Star::default()),
                                BinOp::Sub(_) => BinOp::Div(token::Slash::default()),
                                BinOp::Mul(_) => BinOp::Add(token::Plus::default()),
                                BinOp::Div(_) => BinOp::Sub(token::Minus::default()),
                                _ => operator.clone(),
                            };

                            *expr = Expr::Binary(ExprBinary {
                                attrs: binary_expr.attrs.clone(),
                                left: left.clone(),
                                op: new_operator,
                                right: right.clone(),
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator substitutes generic const expressions with different arithmetic operations to test the compiler's handling of various operations within the context of generic const expressions."
    }
}