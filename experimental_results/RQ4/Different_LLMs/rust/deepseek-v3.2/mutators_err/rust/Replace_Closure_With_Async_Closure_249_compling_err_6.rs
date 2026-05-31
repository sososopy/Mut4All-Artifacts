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

pub struct Replace_Closure_With_Async_Closure_249;

impl Mutator for Replace_Closure_With_Async_Closure_249 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Async_Closure_249"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces any non-async closure expression with an async closure by adding the async keyword before the closure's parameter list or move keyword. This transformation aims to stress the compiler's async closure handling, particularly in MIR dumping and coroutine transformation passes, potentially triggering internal errors related to async closure processing."
    }
}

struct ClosureVisitor;

impl VisitMut for ClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            if closure.asyncness.is_none() {
                closure.asyncness = Some(token::Async {
                    span: Span::call_site(),
                });
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}