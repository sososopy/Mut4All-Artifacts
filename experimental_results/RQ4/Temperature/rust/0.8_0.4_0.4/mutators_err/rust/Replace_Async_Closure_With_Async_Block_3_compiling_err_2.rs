use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Replace_Async_Closure_With_Async_Block_3;

impl Mutator for Replace_Async_Closure_With_Async_Block_3 {
    fn name(&self) -> &str {
        "Replace_Async_Closure_With_Async_Block_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut visitor = AsyncClosureReplacer;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures and replaces them with equivalent async blocks. This transformation helps in identifying issues related to async-await handling by forcing the compiler to process the logic differently, potentially uncovering bugs or unexpected behaviors."
    }
}

struct AsyncClosureReplacer;

impl VisitMut for AsyncClosureReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(expr_closure) = expr {
            if expr_closure.asyncness.is_some() {
                let async_block: Expr = parse_quote! {
                    async {
                        #(#expr_closure.inputs),*;
                        #expr_closure.body
                    }
                };
                *expr = async_block;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}