use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprClosure, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatIdent, PatType, Path as SynPath,
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

pub struct Generator_Yield_Dereference_NonSized_118;

impl Mutator for Generator_Yield_Dereference_NonSized_118 {
    fn name(&self) -> &str {
        "Generator_Yield_Dereference_NonSized_118"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = GeneratorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies generator closures and replaces an expression with a block containing a yield followed by a dereference of a non-sized type variable. It introduces a new variable if none exists, potentially exposing ICEs related to layout calculations for generators with non-sized types after yield."
    }
}

struct GeneratorVisitor;

impl VisitMut for GeneratorVisitor {
    fn visit_expr_closure_mut(&mut self, expr: &mut ExprClosure) {
        if has_yield_in_expr(&expr.body) {
            if let Expr::Block(expr_block) = &mut *expr.body {
                let block = &mut expr_block.block;
                let var_name = find_non_sized_variable(block).unwrap_or_else(|| {
                    let new_var = Ident::new("s", Span::call_site());
                    let new_local = Local {
                        attrs: vec![],
                        pat: Pat::Ident(PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: new_var.clone(),
                            subpat: None,
                        }),
                        let_token: token::Let::default(),
                        init: Some(LocalInit {
                            eq_token: token::Eq::default(),
                            expr: Box::new(Expr::Lit(ExprLit {
                                attrs: vec![],
                                lit: Lit::Str(LitStr::new("", Span::call_site())),
                            })),
                            diverge: None,
                        }),
                        semi_token: token::Semi::default(),
                    };
                    block.stmts.insert(0, Stmt::Local(new_local));
                    new_var
                });
                if let Some(last_stmt) = block.stmts.last_mut() {
                    if let Stmt::Expr(expr, _) = last_stmt {
                        let new_block = parse_quote!({
                            yield;
                            *#var_name
                        });
                        *expr = new_block;
                    }
                }
            }
        }
        self.visit_expr_mut(&mut expr.body);
    }
}

fn has_yield_in_expr(expr: &Expr) -> bool {
    let mut visitor = YieldChecker::new();
    visitor.visit_expr(expr);
    visitor.found
}

struct YieldChecker {
    found: bool,
}

impl YieldChecker {
    fn new() -> Self {
        Self { found: false }
    }
}

impl<'a> Visit<'a> for YieldChecker {
    fn visit_expr_yield(&mut self, _: &ExprYield) {
        self.found = true;
    }
}

fn find_non_sized_variable(block: &Block) -> Option<Ident> {
    for stmt in &block.stmts {
        if let Stmt::Local(local) = stmt {
            if let Pat::Type(pat_type) = &local.pat {
                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                    if is_non_sized_type(&Some(pat_type.ty.clone())) {
                        return Some(pat_ident.ident.clone());
                    }
                }
            }
        }
    }
    None
}

fn is_non_sized_type(ty: &Option<Box<Type>>) -> bool {
    if let Some(ty) = ty {
        if let Type::Reference(type_ref) = &**ty {
            if let Type::Path(type_path) = &*type_ref.elem {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident == "str" {
                        return true;
                    }
                    if segment.ident == "[" {
                        return true;
                    }
                }
            }
        }
    }
    false
}