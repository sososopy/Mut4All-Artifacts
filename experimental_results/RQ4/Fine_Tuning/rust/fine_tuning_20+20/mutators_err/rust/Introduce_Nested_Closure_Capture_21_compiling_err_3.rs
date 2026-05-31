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

pub struct Introduce_Nested_Closure_Capture_21;

impl Mutator for Introduce_Nested_Closure_Capture_21 {
    fn name(&self) -> &str {
        "Introduce_Nested_Closure_Capture_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureVisitor::new();
                visitor.visit_item_fn(item_fn);
                if let Some(closure_expr) = visitor.target_closure {
                    let mut nested_closure = closure_expr.clone();
                    if let Expr::Closure(nested_closure_expr) = &mut nested_closure {
                        if !nested_closure_expr.inputs.is_empty() {
                            let input_pat = &nested_closure_expr.inputs[0];
                            let input_ident = match input_pat {
                                Pat::Ident(pat_ident) => Some(pat_ident.ident.clone()),
                                _ => None,
                            };
                            if let Some(input_ident) = input_ident {
                                let nested_body: Expr = parse_quote! {
                                    |nested_var| { #input_ident + nested_var }
                                };
                                nested_closure_expr.body = Box::new(nested_body);
                            }
                        }
                    }
                    let nested_call: Expr = parse_quote! {
                        #nested_closure(2)
                    };
                    let new_block: Expr = parse_quote! {
                        {
                            let nested_closure = #nested_closure;
                            #nested_call;
                        }
                    };
                    closure_expr.body = Box::new(new_block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets closures within functions and introduces a nested closure that captures a variable from its enclosing closure. By adding a nested closure that interacts with captured variables, it stresses the compiler's closure handling and variable capture mechanisms, potentially exposing issues in how the compiler resolves and manages nested closure environments."
    }
}

struct ClosureVisitor {
    target_closure: Option<ExprClosure>,
}

impl ClosureVisitor {
    fn new() -> Self {
        ClosureVisitor {
            target_closure: None,
        }
    }
}

impl<'ast> Visit<'ast> for ClosureVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Closure(closure_expr) = node {
            self.target_closure = Some(closure_expr.clone());
        }
        syn::visit::visit_expr(self, node);
    }
}