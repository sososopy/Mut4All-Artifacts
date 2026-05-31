use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprClosure, ExprYield, File, FnArg, GenericArgument,
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned,
    token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Replace_Closure_Body_With_Yield_12;

struct YieldFinder {
    found: bool,
}

impl<'ast> Visit<'ast> for YieldFinder {
    fn visit_expr_yield(&mut self, _: &ExprYield) {
        self.found = true;
    }
}

struct ClosureReplacer;

impl VisitMut for ClosureReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            let new_block = parse_quote! { { yield () } };
            closure.body = Box::new(Expr::Block(ExprBlock {
                attrs: vec![],
                block: new_block,
                label: None,
            }));
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}

impl Mutator for Replace_Closure_Body_With_Yield_12 {
    fn name(&self) -> &str {
        "Replace_Closure_Body_With_Yield_12"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut yield_finder = YieldFinder { found: false };
                yield_finder.visit_block(&func.block);
                if yield_finder.found {
                    let mut replacer = ClosureReplacer;
                    replacer.visit_block_mut(&mut func.block);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the body of closures within generator functions with a `yield ()` expression. This introduces a yield inside a closure, which is valid only if the closure is part of a generator. The transformation aims to test the compiler's handling of nested generators and closures, potentially exposing ICEs when type-checking or validating generator lifetimes and closure captures."
    }
}