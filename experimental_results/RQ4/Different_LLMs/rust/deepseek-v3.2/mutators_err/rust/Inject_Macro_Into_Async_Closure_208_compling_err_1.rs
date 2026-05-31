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

pub struct Inject_Macro_Into_Async_Closure_208;

impl Mutator for Inject_Macro_Into_Async_Closure_208 {
    fn name(&self) -> &str {
        "Inject_Macro_Into_Async_Closure_208"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = AsyncClosureVisitor {
            rng: &mut rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async closures and injects a macro invocation into their body. It first collects all macro invocations present in the seed program to reuse existing macros. If none exist, it introduces a simple println! macro with a use statement. For each async closure, if the body is a block, the macro call is inserted as the first statement; if it's an expression, the body is wrapped in a block with the macro call preceding the expression. This transformation stresses the compiler's handling of async closures with macro expansions, potentially triggering hygiene issues, macro resolution errors, or async/await desugaring bugs."
    }
}

struct AsyncClosureVisitor<'a, R: Rng> {
    rng: &'a mut R,
    mutated: bool,
}

impl<'a, R: Rng> VisitMut for AsyncClosureVisitor<'a, R> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            if closure.asyncness.is_some() {
                let mut macro_calls = Vec::new();
                self.collect_macro_calls(expr, &mut macro_calls);
                
                let macro_stmt = if !macro_calls.is_empty() {
                    let selected = macro_calls.choose(self.rng).unwrap();
                    parse_quote!(#selected;)
                } else {
                    parse_quote!(println!("");)
                };
                
                match &mut *closure.body {
                    Expr::Block(block) => {
                        block.block.stmts.insert(0, macro_stmt);
                        self.mutated = true;
                    }
                    _ => {
                        let old_body = std::mem::replace(&mut *closure.body, parse_quote!({}));
                        *closure.body = parse_quote!({
                            #macro_stmt
                            #old_body
                        });
                        self.mutated = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl<'a, R: Rng> AsyncClosureVisitor<'a, R> {
    fn collect_macro_calls(&mut self, expr: &Expr, calls: &mut Vec<Expr>) {
        match expr {
            Expr::Macro(expr_macro) => {
                calls.push(Expr::Macro(expr_macro.clone()));
            }
            Expr::Call(ExprCall { func, .. }) => {
                if let Expr::Path(ExprPath { path, .. }) = &**func {
                    if path.segments.last().map_or(false, |seg| seg.ident.to_string().ends_with('!')) {
                        calls.push(expr.clone());
                    }
                }
            }
            _ => {
                syn::visit::visit_expr(self, expr);
            }
        }
    }
}

impl<'a, R: Rng> Visit for AsyncClosureVisitor<'a, R> {
    fn visit_expr(&mut self, expr: &Expr) {
        self.collect_macro_calls(expr, &mut Vec::new());
        syn::visit::visit_expr(self, expr);
    }
}