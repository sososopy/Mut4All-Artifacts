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

pub struct Array_Size_Const_Expr_Arithmetic_Mutation_368;

impl Mutator for Array_Size_Const_Expr_Arithmetic_Mutation_368 {
    fn name(&self) -> &str {
        "Array_Size_Const_Expr_Arithmetic_Mutation_368"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ArraySizeMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ArraySizeMutatorVisitor;

impl<'ast> VisitMut for ArraySizeMutatorVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(array) = ty {
            self.visit_type_array_mut(array);
        } else {
            visit_mut::visit_type_mut(self, ty);
        }
    }

    fn visit_type_array_mut(&mut self, array: &mut TypeArray) {
        visit_mut::visit_type_array_mut(self, array);
        if let Expr::Binary(expr_binary) = array.len.as_mut() {
            match expr_binary.op {
                BinOp::Mul(_) => {
                    let original_span = expr_binary.op.span();
                    let new_token = token::Plus::default();
                    let new_add_op = BinOp::Add(token::Plus::default());
                    expr_binary.op = new_add_op;
                },
                BinOp::Add(_) => {
                    let original_span = expr_binary.op.span();
                    let new_token = token::Star::default();
                    let new_mul_op = BinOp::Mul(token::Star::default());
                    expr_binary.op = new_mul_op;
                },
                BinOp::Div(_) => {
                    let original_span = expr_binary.op.span();
                    let new_token = token::Percent::default();
                    let new_rem_op = BinOp::Rem(token::Percent::default());
                    expr_binary.op = new_rem_op;
                },
                BinOp::Rem(_) => {
                    let original_span = expr_binary.op.span();
                    let new_token = token::Slash::default();
                    let new_div_op = BinOp::Div(token::Slash::default());
                    expr_binary.op = new_div_op;
                },
                BinOp::Sub(_) => {
                    let original_span = expr_binary.op.span();
                    let new_token = token::Plus::default();
                    let new_add_op = BinOp::Add(token::Plus::default());
                    expr_binary.op = new_add_op;
                },
                _ => {}
            }
        }
    }
}