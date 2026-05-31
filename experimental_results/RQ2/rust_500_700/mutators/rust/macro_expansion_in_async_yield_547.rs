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

pub struct Macro_Expansion_In_Async_Yield_547;

impl Mutator for Macro_Expansion_In_Async_Yield_547 {
    fn name(&self) -> &str {
        "Macro_Expansion_In_Async_Yield_547"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = AsyncYieldMacroVisitor::new();
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async blocks containing yield expressions and macro calls. It rearranges these elements to challenge the compiler's handling of async coroutines and macro expansion."
    }
}

struct AsyncYieldMacroVisitor {
    has_yield: bool,
    macro_calls: Vec<Expr>,
}

impl AsyncYieldMacroVisitor {
    fn new() -> Self {
        Self {
            has_yield: false,
            macro_calls: Vec::new(),
        }
    }
}

impl VisitMut for AsyncYieldMacroVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Async(expr_async) => {
                self.has_yield = false;
                self.macro_calls.clear();
                syn::visit_mut::visit_block_mut(self, &mut expr_async.block);
                if self.has_yield && !self.macro_calls.is_empty() {
                    let mut new_stmts = Vec::new();
                    for macro_call in &self.macro_calls {
                        new_stmts.push(Stmt::Expr(macro_call.clone(), None));
                    }
                    new_stmts.push(Stmt::Expr(parse_quote!(yield), None));
                    new_stmts.push(Stmt::Expr(parse_quote!(another_macro!()), None));
                    expr_async.block.stmts = new_stmts;
                }
            }
            Expr::Yield(_) => {
                self.has_yield = true;
            }
            Expr::Macro(expr_macro) => {
                self.macro_calls.push(Expr::Macro(expr_macro.clone()));
            }
            _ => syn::visit_mut::visit_expr_mut(self, expr),
        }
    }
}