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

pub struct Mutator_Lambda_Yield_280;

impl Mutator for Mutator_Lambda_Yield_280 {
    fn name(&self) -> &str {
        "Mutator_Lambda_Yield_280"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LambdaVisitor;

        impl VisitMut for LambdaVisitor {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if let Expr::Block(block) = &mut *i.body {
                    let yield_expr: Expr = parse_quote! { yield };
                    let return_expr: Expr = parse_quote! { return };
                    
                    if block.block.stmts.iter().any(|stmt| matches!(stmt, Stmt::Expr(Expr::Yield(_), _))) {
                        block.block.stmts = vec![Stmt::Expr(return_expr, None)];
                    } else {
                        block.block.stmts = vec![Stmt::Expr(yield_expr, None)];
                    }
                }
                syn::visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        let mut visitor = LambdaVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets lambda expressions within functions. It modifies the body of a lambda expression to include a yield statement if it doesn't already contain one. If the lambda already contains a yield, it replaces it with a return statement. This transformation can introduce generator-related type-checking issues in the compiler."
    }
}