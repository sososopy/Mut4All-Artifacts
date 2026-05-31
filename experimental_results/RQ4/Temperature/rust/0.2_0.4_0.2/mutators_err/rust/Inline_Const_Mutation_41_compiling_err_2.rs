use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Inline_Const_Mutation_41;

impl Mutator for Inline_Const_Mutation_41 {
    fn name(&self) -> &str {
        "Inline_Const_Mutation_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = InlineConstVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets inline constant blocks within functions. It introduces additional operations or function calls within these blocks to test the compiler's handling of more complex expressions. By using existing variables or introducing new ones, it aims to increase the complexity of inline constant evaluations."
    }
}

struct InlineConstVisitor;

impl VisitMut for InlineConstVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Block(ExprBlock { block, .. }) = expr {
            if block.stmts.iter().any(|stmt| matches!(stmt, Stmt::Local(Local { let_token, .. }) if let_token.is_some())) {
                let new_stmts: Vec<Stmt> = vec![
                    parse_quote! { let x: u8 = 5; },
                    parse_quote! { let y: u8 = 10; },
                    parse_quote! { let result = x + y; },
                ];
                block.stmts.splice(0..0, new_stmts);
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}