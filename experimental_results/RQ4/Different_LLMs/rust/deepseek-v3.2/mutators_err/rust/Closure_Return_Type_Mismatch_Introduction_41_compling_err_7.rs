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

pub struct Closure_Return_Type_Mismatch_Introduction_41;

impl Mutator for Closure_Return_Type_Mismatch_Introduction_41 {
    fn name(&self) -> &str {
        "Closure_Return_Type_Mismatch_Introduction_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureMutatorVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure expressions and modifies their bodies to return a mismatched type. It analyzes the closure's body expression and replaces it with a syntactically valid but type-incompatible expression, using available variables/literals or introducing default values like (), true, false, \"\", or 0.0. This creates deliberate type conflicts to stress the compiler's type-checking and error-handling for closures, potentially triggering ICEs or unexpected behaviors."
    }
}

struct ClosureMutatorVisitor {
    rng: rand::rngs::ThreadRng,
}

impl ClosureMutatorVisitor {
    fn new() -> Self {
        Self {
            rng: thread_rng(),
        }
    }

    fn get_mismatched_expr(&mut self, span: Span) -> Expr {
        let choices: [Expr; 5] = [
            parse_quote! { () },
            parse_quote! { true },
            parse_quote! { false },
            parse_quote! { "" },
            parse_quote! { 0.0 },
        ];
        let mut expr = choices.choose(&mut self.rng).unwrap().clone();
        match &mut expr {
            Expr::Tuple(tuple) => tuple.elems.set_span(span),
            Expr::Lit(lit) => lit.lit.set_span(span),
            _ => {}
        }
        expr
    }
}

impl VisitMut for ClosureMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            let closure_span = closure.span();
            if let Expr::Block(block) = &mut *closure.body {
                if let Some(last_stmt) = block.block.stmts.last_mut() {
                    if let Stmt::Expr(last_expr, _) = last_stmt {
                        *last_expr = self.get_mismatched_expr(last_expr.span());
                    }
                } else {
                    let new_expr = self.get_mismatched_expr(closure_span);
                    block.block.stmts.push(Stmt::Expr(new_expr, None));
                }
            } else {
                let new_expr = self.get_mismatched_expr(closure.body.span());
                closure.body = Box::new(new_expr);
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}