use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Lifetime, Ident,
    Item, ItemFn, ItemStruct, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

use syn::Lifetime as SynLifetime;
use crate::mutator::Mutator;

pub struct Nested_Closure_Lifetime_Capture_40;

impl Mutator for Nested_Closure_Lifetime_Capture_40 {
    fn name(&self) -> &str {
        "Nested_Closure_Lifetime_Capture_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureMutatorVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested closures where the inner closure captures a lifetime from the outer closure. It introduces a new lifetime parameter in the outer closure if absent and replaces the inner closure's body with a new closure that uses this lifetime. This tests the compiler's handling of nested binder propagation and lifetime elision in closure expressions."
    }
}

struct ClosureMutatorVisitor;

impl ClosureMutatorVisitor {
    fn new() -> Self {
        Self
    }

    fn process_closure(&self, closure: &mut ExprClosure) {
        let mut inner_visitor = InnerClosureFinder::new();
        inner_visitor.visit_expr_mut(&mut closure.body);
        if let Some(inner) = inner_visitor.found {
            self.mutate_outer_and_inner(closure, inner);
        }
    }

    fn mutate_outer_and_inner(&self, outer: &mut ExprClosure, inner: Box<ExprClosure>) {
        if outer.capture.is_none() {
            let new_bound_lifetimes = BoundLifetimes {
                for_token: token::For::default(),
                lifetimes: Punctuated::new(),
                lt_token: token::Lt::default(),
                gt_token: token::Gt::default(),
            };
            new_bound_lifetimes.lifetimes.push(Lifetime(LifetimeParam {
                lifetime: SynLifetime::new("'x", proc_macro2::Span::call_site()),
                colon_token: None,
                attrs: Vec::new(),
                bounds: Punctuated::new(),
            }));
            outer.capture = Some(syn::ClosureCapture::ForLifetimes(new_bound_lifetimes));
        }
        let new_inner = parse_quote! {
            for<'x> |_: &'x ()| -> () {
                // empty body
            }
        };
        *outer.body = Expr::Closure(new_inner);
    }
}

struct InnerClosureFinder {
    found: Option<Box<ExprClosure>>,
}

impl InnerClosureFinder {
    fn new() -> Self {
        Self { found: None }
    }
}

impl VisitMut for InnerClosureFinder {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            let new_closure = parse_quote! { || {} };
            let expr_clone = std::mem::replace(expr, Expr::Closure(new_closure));
            self.found = Some(Box::new(ExprClosure::from(expr_clone)));
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl VisitMut for ClosureMutatorVisitor {
    fn visit_expr_closure_mut(&mut self, closure: &mut ExprClosure) {
        self.process_closure(closure);
        syn::visit_mut::visit_expr_closure_mut(self, closure);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}