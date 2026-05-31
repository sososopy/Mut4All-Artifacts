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

pub struct Async_Closure_To_Async_Block_249;

impl Mutator for Async_Closure_To_Async_Block_249 {
    fn name(&self) -> &str {
        "Async_Closure_To_Async_Block_249"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut visitor = AsyncClosureVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms async closures into async blocks. This involves removing the closure syntax `||` and enclosing the body within an `async {}` block. This transformation tests the compiler's handling of async constructs by replacing unstable async closures with stable async blocks."
    }
}

struct AsyncClosureVisitor;

impl VisitMut for AsyncClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(expr_closure) = expr {
            if expr_closure.asyncness.is_some() {
                let async_block: Expr = parse_quote! {
                    async {
                        #(#expr_closure.body)*
                    }
                };
                *expr = async_block;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}