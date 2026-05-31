use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAssign, ExprBinary, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Operator_With_Unicode_235;

impl Mutator for Replace_Operator_With_Unicode_235 {
    fn name(&self) -> &str {
        "Replace_Operator_With_Unicode_235"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = OperatorReplacer { in_binary: false, in_statement: false };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct OperatorReplacer {
    in_binary: bool,
    in_statement: bool,
}

impl VisitMut for OperatorReplacer {
    fn visit_expr_binary_mut(&mut self, expr: &mut ExprBinary) {
        self.in_binary = true;
        syn::visit_mut::visit_expr_binary_mut(self, expr);
        self.in_binary = false;
    }

    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        self.in_statement = true;
        syn::visit_mut::visit_stmt_mut(self, stmt);
        self.in_statement = false;
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Assign(assign_expr) = expr {
            if self.in_statement && !self.in_binary {
                // Avoid replacing if RHS is a binary expression to prevent chained comparisons
                if let Expr::Binary(_) = &*assign_expr.right {
                    return;
                }
                let lhs = &assign_expr.left;
                let rhs = &assign_expr.right;
                let new_expr = parse_quote! { #lhs == #rhs };
                *expr = new_expr;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}