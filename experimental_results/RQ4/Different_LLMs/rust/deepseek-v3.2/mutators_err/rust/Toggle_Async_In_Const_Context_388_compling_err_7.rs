use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAsync, ExprBlock, ExprCall, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Toggle_Async_In_Const_Context_388;

impl Mutator for Toggle_Async_In_Const_Context_388 {
    fn name(&self) -> &str {
        "Toggle_Async_In_Const_Context_388"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ToggleAsyncVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator toggles the async keyword on blocks and closures that appear within constant expression contexts, such as array length expressions, const generic arguments, or associated const values. It transforms async blocks/closures to non-async and vice versa, targeting the interaction between async constructs and constant evaluation. This can expose internal compiler errors where the compiler expects a coroutine type in a const context."
    }
}

struct ToggleAsyncVisitor;

impl VisitMut for ToggleAsyncVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Async(expr_async) => {
                if self.is_in_const_context(expr_async) {
                    let block = expr_async.block.clone();
                    *expr = Expr::Block(ExprBlock {
                        attrs: expr_async.attrs.clone(),
                        label: None,
                        block,
                    });
                }
            }
            Expr::Closure(expr_closure) => {
                if self.is_in_const_context(expr_closure) {
                    expr_closure.asyncness = if expr_closure.asyncness.is_some() {
                        None
                    } else {
                        Some(token::Async::default())
                    };
                }
            }
            Expr::Block(expr_block) => {
                if self.is_in_const_context(expr_block) {
                    let attrs = expr_block.attrs.clone();
                    let block = expr_block.block.clone();
                    *expr = Expr::Async(ExprAsync {
                        attrs,
                        async_token: token::Async::default(),
                        capture: None,
                        block,
                    });
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl ToggleAsyncVisitor {
    fn is_in_const_context<T: Spanned>(&self, node: &T) -> bool {
        let span = node.span();
        let source = span.source_text().unwrap_or_default();
        let const_keywords = ["const", "CONST"];
        for keyword in const_keywords {
            if source.contains(keyword) {
                return true;
            }
        }
        false
    }
}