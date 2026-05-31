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

pub struct Nested_Generator_Mutation_661;

impl Mutator for Nested_Generator_Mutation_661 {
    fn name(&self) -> &str {
        "Nested_Generator_Mutation_661"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = GeneratorVisitor::default();
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets nested generators within a function, modifying the nesting structure or yield points. It aims to test the compiler's handling of complex generator structures, borrow, and ownership rules."
    }
}

#[derive(Default)]
struct GeneratorVisitor;

impl VisitMut for GeneratorVisitor {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        if let Expr::Block(block) = &mut *node.body {
            let mut rng = thread_rng();
            if rng.gen_bool(0.5) {
                // Add an extra yield before existing yields
                let new_yield: Stmt = parse_quote! { yield; };
                block.block.stmts.insert(0, new_yield);
            } else {
                // Add a new layer of nesting
                let inner_closure: Expr = parse_quote! {
                    || {
                        yield;
                    }
                };
                let new_stmt: Stmt = parse_quote! {
                    let mut inner = #inner_closure;
                };
                block.block.stmts.push(new_stmt);
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, node);
    }
}