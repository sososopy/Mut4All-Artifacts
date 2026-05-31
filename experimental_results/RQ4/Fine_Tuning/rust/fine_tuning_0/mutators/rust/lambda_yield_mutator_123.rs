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

pub struct Lambda_Yield_Mutator_123;

impl Mutator for Lambda_Yield_Mutator_123 {
    fn name(&self) -> &str {
        "Lambda_Yield_Mutator_123"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = LambdaVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions containing lambda expressions. It modifies the body of the lambda to include a `yield` expression, potentially introducing generator-related type-checking issues."
    }
}

struct LambdaVisitor;

impl VisitMut for LambdaVisitor {
    fn visit_expr_closure_mut(&mut self, closure: &mut ExprClosure) {
        if let Expr::Yield(_) = *closure.body {
            // If the closure already contains a yield, replace it with a return statement
            closure.body = Box::new(parse_quote!(return));
        } else {
            // Otherwise, introduce a yield expression
            closure.body = Box::new(parse_quote!(yield));
        }
        // Continue visiting nested expressions
        syn::visit_mut::visit_expr_closure_mut(self, closure);
    }
}