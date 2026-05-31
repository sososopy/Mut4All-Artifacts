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

pub struct Append_Await_To_Function_Calls_229;

impl Mutator for Append_Await_To_Function_Calls_229 {
    fn name(&self) -> &str {
        "Append_Await_To_Function_Calls_229"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    let mut visitor = FunctionCallVisitor::default();
                    visitor.visit_block_mut(&mut item_fn.block);
                }
                Item::Impl(item_impl) => {
                    for item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(item_fn) = item {
                            let mut visitor = FunctionCallVisitor::default();
                            visitor.visit_block_mut(&mut item_fn.block);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all function calls in free functions and inherent methods, appending `.await{}` to each call expression. By introducing invalid await syntax, it aims to stress-test the compiler's async-await parsing and error recovery mechanisms, potentially uncovering bugs in how the compiler handles erroneous await expressions."
    }
}

#[derive(Default)]
struct FunctionCallVisitor {
    call_spans: Vec<Span>,
}

impl VisitMut for FunctionCallVisitor {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Call(expr_call) = node {
            self.call_spans.push(expr_call.span());
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
    fn visit_block_mut(&mut self, node: &mut syn::Block) {
        self.call_spans.clear();
        syn::visit_mut::visit_block_mut(self, node);
        for &span in &self.call_spans {
            let expr = Expr::Verbatim(quote_spanned!(span=> .await{}));
            node.stmts.push(Stmt::Expr(expr, None));
        }
    }
}