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

pub struct Modify_Macro_Inside_Async_Closure_208;

impl Mutator for Modify_Macro_Inside_Async_Closure_208 {
    fn name(&self) -> &str {
        "Modify_Macro_Inside_Async_Closure_208"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = AsyncClosureMacroVisitor::new();
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures within functions. It identifies macro calls within these closures and modifies them by introducing an undefined variable as a parameter, aiming to provoke macro expansion errors and test the compiler's resilience to unexpected macro patterns."
    }
}

struct AsyncClosureMacroVisitor;

impl AsyncClosureMacroVisitor {
    fn new() -> Self {
        AsyncClosureMacroVisitor
    }
}

impl VisitMut for AsyncClosureMacroVisitor {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        if node.asyncness.is_some() {
            if let Expr::Block(expr_block) = &mut *node.body {
                for stmt in &mut expr_block.block.stmts {
                    if let Stmt::Expr(Expr::Macro(expr_macro), _) = stmt {
                        let new_macro: Expr = parse_quote! {
                            println!("{:?}", undefined_var)
                        };
                        *stmt = Stmt::Expr(new_macro, None);
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, node);
    }
}