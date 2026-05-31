use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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
use rand::rngs::ThreadRng;

pub struct Replace_Bool_Const_Generic_With_Numeric_466;

impl Mutator for Replace_Bool_Const_Generic_With_Numeric_466 {
    fn name(&self) -> &str {
        "Replace_Bool_Const_Generic_With_Numeric_466"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut collector = NumericExprCollector::default();
        collector.visit_file(file);
        let numeric_exprs: Vec<_> = collector.exprs.into_iter().collect();
        if numeric_exprs.is_empty() {
            return;
        }

        let mut visitor = FunctionCallVisitor {
            numeric_exprs,
            rng: &mut thread_rng(),
        };
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces boolean-typed const generic parameters with numeric expressions (e.g., 0u8, 1i32) in generic function invocations. This transformation introduces type mismatches in const parameters, potentially exposing compiler bugs in const generic evaluation and type consistency checks."
    }
}

struct NumericExprCollector {
    exprs: Vec<Expr>,
}

impl Default for NumericExprCollector {
    fn default() -> Self {
        Self { exprs: Vec::new() }
    }
}

impl Visit<'_> for NumericExprCollector {
    fn visit_expr_lit(&mut self, node: &ExprLit) {
        if let Lit::Int(_) = node.lit {
            self.exprs.push(Expr::Lit(node.clone()));
        } else if let Lit::Float(_) = node.lit {
            self.exprs.push(Expr::Lit(node.clone()));
        }
        syn::visit::visit_expr_lit(self, node);
    }
}

struct FunctionCallVisitor<'a> {
    numeric_exprs: Vec<Expr>,
    rng: &'a mut ThreadRng,
}

impl<'a> FunctionCallVisitor<'a> {
    fn get_random_expr(&mut self) -> Expr {
        if self.numeric_exprs.is_empty() {
            return parse_quote!(0u8);
        }
        let idx = self.rng.gen_range(0..self.numeric_exprs.len());
        self.numeric_exprs[idx].clone()
    }
}

impl<'a> VisitMut for FunctionCallVisitor<'a> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(expr_path) = &*node.func {
            for segment in &mut expr_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Lit(lit) = &*const_arg.expr {
                                if let Lit::Bool(_) = lit.lit {
                                    let replacement = self.get_random_expr();
                                    *const_arg.expr = Box::new(replacement);
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}