use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAssign, ExprAwait, ExprCall, ExprPath, ExprUnary, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, UnOp, parse_quote,
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

pub struct Replace_Await_Assignment_Deref_Address_16;

impl Mutator for Replace_Await_Assignment_Deref_Address_16 {
    fn name(&self) -> &str {
        "Replace_Await_Assignment_Deref_Address_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AwaitAssignmentVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets assignment statements where the left-hand side is an await expression yielding a mutable reference (e.g., `*some_async_expr.await = value;`). It transforms it to `*(&mut some_async_expr).await = value;`, wrapping the awaited expression in a mutable borrow before awaiting. This changes the borrowing and drop tracking behavior, potentially affecting lifetimes and the order of operations. The mutation only applies within async contexts and preserves the original expressions without introducing new names."
    }
}

struct AwaitAssignmentVisitor;

impl VisitMut for AwaitAssignmentVisitor {
    fn visit_expr_assign_mut(&mut self, expr: &mut ExprAssign) {
        if let Expr::Unary(unary_expr) = &*expr.left {
            if let UnOp::Deref(_) = unary_expr.op {
                if let Expr::Await(await_expr) = &*unary_expr.expr {
                    let inner_expr = &await_expr.base;
                    let new_await_expr: Expr = parse_quote! {
                        (&mut #inner_expr).await
                    };
                    let new_deref_expr: Expr = parse_quote! {
                        *#new_await_expr
                    };
                    expr.left = Box::new(new_deref_expr);
                }
            }
        }
        syn::visit_mut::visit_expr_assign_mut(self, expr);
    }
}