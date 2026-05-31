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

pub struct Async_Closure_To_Async_Block_249;

impl Mutator for Async_Closure_To_Async_Block_249 {
    fn name(&self) -> &str {
        "Async_Closure_To_Async_Block_249"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor;
        impl VisitMut for ClosureVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(expr_closure) = expr {
                    if expr_closure.asyncness.is_some() {
                        let body = expr_closure.body.clone();
                        *expr = Expr::Async(ExprAsync {
                            attrs: expr_closure.attrs.clone(),
                            async_token: expr_closure.asyncness.unwrap(),
                            capture: expr_closure.capture,
                            block: Block {
                                brace_token: token::Brace::default(),
                                stmts: vec![Stmt::Expr(*body, None)],
                            },
                        });
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async closures and transforms them into async blocks. By replacing the closure syntax with an async block, it leverages the stable async block feature to stress-test the compiler's handling of async constructs, especially in contexts where async closures might be used."
    }
}