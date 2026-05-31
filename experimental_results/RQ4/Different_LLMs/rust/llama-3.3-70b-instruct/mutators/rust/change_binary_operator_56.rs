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
    token::{Minus, Percent, Plus, Shl, Shr, Slash, Star},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Change_Binary_Operator_56;

impl Mutator for Change_Binary_Operator_56 {
    fn name(&self) -> &str {
        "Change_Binary_Operator_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Binary(binary_expr) = expr {
                            let mut rng = thread_rng();
                            let operators: Vec<syn::BinOp> = vec![
                                syn::BinOp::Add(Plus { spans: [binary_expr.op.span()] }),
                                syn::BinOp::Sub(Minus { spans: [binary_expr.op.span()] }),
                                syn::BinOp::Mul(Star { spans: [binary_expr.op.span()] }),
                                syn::BinOp::Div(Slash { spans: [binary_expr.op.span()] }),
                                syn::BinOp::Rem(Percent { spans: [binary_expr.op.span()] }),
                                syn::BinOp::Shl(Shl { spans: [binary_expr.op.span(), binary_expr.op.span()] }),
                                syn::BinOp::Shr(Shr { spans: [binary_expr.op.span(), binary_expr.op.span()] }),
                            ];
                            let new_operator = operators.choose(&mut rng).unwrap();
                            binary_expr.op = *new_operator;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes binary operators in the given code. It replaces the operator with a randomly chosen one from a list of binary operators, including addition, subtraction, multiplication, division, modulo, left shift, and right shift. This transformation aims to test the compiler's handling of different binary operations and its ability to correctly evaluate expressions and handle potential errors."
    }
}