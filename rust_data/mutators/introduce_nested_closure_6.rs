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

pub struct Introduce_Nested_Closure_6;

impl Mutator for Introduce_Nested_Closure_6 {
    fn name(&self) -> &str {
        "Introduce_Nested_Closure_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureVisitor::new();
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within functions, introducing a nested closure that captures a variable from its enclosing closure. This stresses the compiler's handling of variable captures and closure nesting. The mutation involves identifying a closure, adding a nested closure inside it, and ensuring the nested closure performs operations involving the captured variable."
    }
}

struct ClosureVisitor {
    closure_count: usize,
}

impl ClosureVisitor {
    fn new() -> Self {
        ClosureVisitor { closure_count: 0 }
    }
}

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
        if self.closure_count == 0 {
            if let Some(first_input) = i.inputs.first() {
                if let Pat::Ident(pat_ident) = first_input {
                    let var_ident = &pat_ident.ident;
                    let nested_closure: ExprClosure = parse_quote! {
                        |nested_var| {
                            #var_ident + nested_var
                        }
                    };
                    let nested_call: Stmt = parse_quote! {
                        let _ = #nested_closure(2);
                    };
                    if let Expr::Block(expr_block) = &mut *i.body {
                        expr_block.block.stmts.insert(0, nested_call);
                    }
                }
            }
            self.closure_count += 1;
        }
        syn::visit_mut::visit_expr_closure_mut(self, i);
    }
}