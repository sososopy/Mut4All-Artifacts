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

pub struct Swap_Const_WhereClause_Arithmetic_Operator_394;

impl Mutator for Swap_Const_WhereClause_Arithmetic_Operator_394 {
    fn name(&self) -> &str {
        "Swap_Const_WhereClause_Arithmetic_Operator_394"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let operators = vec!["+", "-", "*", "/", "%"];
        let mut visitor = SwapConstWhereClauseArithmeticOperatorVisitor {
            rng: &mut rng,
            operators: &operators,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const expressions in where-clauses. It identifies binary arithmetic operators (+, -, *, /, %) within constant expressions that appear in where-clause bounds like `[(); expr]:`. The operator swaps the arithmetic operator with a different one from the set of valid arithmetic operators, preserving operand types. This transformation aims to trigger internal compiler errors during constant evaluation by introducing mismatched or unexpected operand types across the new operator, potentially exposing bugs in the compiler's const evaluation or type checking logic for generic const expressions."
    }
}

struct SwapConstWhereClauseArithmeticOperatorVisitor<'a, R: Rng> {
    rng: &'a mut R,
    operators: &'a [&'a str],
    mutated: bool,
}

impl<'a, R: Rng> VisitMut for SwapConstWhereClauseArithmeticOperatorVisitor<'a, R> {
    fn visit_where_clause_mut(&mut self, where_clause: &mut WhereClause) {
        for predicate in &mut where_clause.predicates {
            if let WherePredicate::Type(predicate_type) = predicate {
                self.visit_type_mut(&mut predicate_type.bounded_ty);
            }
        }
        syn::visit_mut::visit_where_clause_mut(self, where_clause);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(type_array) = ty {
            if let Expr::Binary(expr_binary) = &mut *type_array.len {
                if self.mutated {
                    return;
                }
                let current_op = match expr_binary.op {
                    BinOp::Add(_) => "+",
                    BinOp::Sub(_) => "-",
                    BinOp::Mul(_) => "*",
                    BinOp::Div(_) => "/",
                    BinOp::Rem(_) => "%",
                    _ => return,
                };
                let mut candidates: Vec<&str> = self.operators.iter().filter(|&&op| op != current_op).cloned().collect();
                if candidates.is_empty() {
                    return;
                }
                let new_op = *candidates.choose(self.rng).unwrap();
                expr_binary.op = match new_op {
                    "+" => BinOp::Add(token::Add::default()),
                    "-" => BinOp::Sub(token::Sub::default()),
                    "*" => BinOp::Mul(token::Mul::default()),
                    "/" => BinOp::Div(token::Div::default()),
                    "%" => BinOp::Rem(token::Rem::default()),
                    _ => unreachable!(),
                };
                self.mutated = true;
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}