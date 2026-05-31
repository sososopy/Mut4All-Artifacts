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

pub struct Async_Function_Await_Manipulation_1;

impl Mutator for Async_Function_Await_Manipulation_1 {
    fn name(&self) -> &str {
        "Async_Function_Await_Manipulation_1"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let mut await_exprs = vec![];
                    let mut visitor = AwaitExprVisitor { await_exprs: &mut await_exprs };
                    visitor.visit_block(&func.block);

                    if await_exprs.len() > 1 {
                        for expr in await_exprs {
                            if let Expr::Await(expr_await) = expr {
                                if let Expr::Reference(expr_ref) = &*expr_await.base {
                                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                                        let new_expr = Expr::Await(ExprAwait {
                                            attrs: vec![],
                                            base: Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: expr_path.path.clone(),
                                            })),
                                            await_token: expr_await.await_token,
                                        });

                                        *expr_await.base = Box::new(new_expr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets async functions with multiple await expressions. It identifies await expressions on the left-hand side of assignments, specifically those awaiting references. The mutation modifies these expressions to await different compatible fields or variables, ensuring the assignment's context remains valid. This explores alternative execution paths and tests the compiler's handling of await expressions."
    }
}

struct AwaitExprVisitor<'a> {
    await_exprs: &'a mut Vec<Expr>,
}

impl<'a> Visit<'a> for AwaitExprVisitor<'a> {
    fn visit_expr(&mut self, node: &'a Expr) {
        if let Expr::Await(_) = node {
            self.await_exprs.push(node.clone());
        }
        syn::visit::visit_expr(self, node);
    }
}