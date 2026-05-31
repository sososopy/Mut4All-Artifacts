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

pub struct Change_Binary_Operator_56;

impl Mutator for Change_Binary_Operator_56 {
    fn name(&self) -> &str {
        "Change_Binary_Operator_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Binary(binary_expr) = expr {
                            let mut rng = thread_rng();
                            let operators = vec![syn::token::Plus, syn::token::Minus, syn::token::Star, syn::token::Slash, syn::token::Percent, syn::token::Caret, syn::token::Shl, syn::token::Shr];
                            let new_operator = operators.choose(&mut rng).unwrap();
                            binary_expr.op = match new_operator {
                                &syn::token::Plus => syn::BinOp::Add(syn::token::Plus { spans: [binary_expr.op.span()] }),
                                &syn::token::Minus => syn::BinOp::Sub(syn::token::Minus { spans: [binary_expr.op.span()] }),
                                &syn::token::Star => syn::BinOp::Mul(syn::token::Star { spans: [binary_expr.op.span()] }),
                                &syn::token::Slash => syn::BinOp::Div(syn::token::Slash { spans: [binary_expr.op.span()] }),
                                &syn::token::Percent => syn::BinOp::Rem(syn::token::Percent { spans: [binary_expr.op.span()] }),
                                &syn::token::Caret => syn::BinOp::Exp(syn::token::Caret { spans: [binary_expr.op.span()] }),
                                &syn::token::Shl => syn::BinOp::Shl(syn::token::Shl { spans: [binary_expr.op.span()] }),
                                &syn::token::Shr => syn::BinOp::Shr(syn::token::Shr { spans: [binary_expr.op.span()] }),
                                _ => unreachable!(),
                            };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes binary operators in the given code. It replaces the operator with a randomly chosen one from a list of binary operators, including addition, subtraction, multiplication, division, modulo, exponentiation, left shift, and right shift. This transformation aims to test the compiler's handling of different binary operations and its ability to correctly evaluate expressions and handle potential errors."
    }
}