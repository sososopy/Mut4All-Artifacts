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

pub struct Insert_Yield_After_VarDecl_116;

impl Mutator for Insert_Yield_After_VarDecl_116 {
    fn name(&self) -> &str {
        "Insert_Yield_After_VarDecl_116"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = GeneratorVisitor {
            yield_insertions: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for (mut block, var_stmt_idx) in visitor.yield_insertions {
            if let Some(stmt) = block.stmts.get(var_stmt_idx) {
                if let Stmt::Local(local) = stmt {
                    let yield_stmt = Stmt::Expr(
                        Expr::Yield(ExprYield {
                            attrs: Vec::new(),
                            yield_token: token::Yield {
                                span: Span::call_site(),
                            },
                            expr: None,
                        }),
                        None,
                    );
                    block.stmts.insert(var_stmt_idx + 1, yield_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies generators (closures containing yield statements) and inserts a yield statement immediately after a local variable declaration, before its first use. This alters control flow and variable liveness across suspension points, potentially exposing drop tracking bugs and incorrect handling of variables that become live across yields."
    }
}

struct GeneratorVisitor {
    yield_insertions: Vec<(Box<Block>, usize)>,
}

impl VisitMut for GeneratorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            let mut yield_present = false;
            let mut var_decls = Vec::new();
            let mut var_uses = HashSet::new();
            let mut stmt_idx = -1;
            if let Expr::Block(block) = &*closure.body {
                for stmt in &block.stmts {
                    stmt_idx += 1;
                    if let Stmt::Expr(Expr::Yield(_), _) = stmt {
                        yield_present = true;
                    }
                    if let Stmt::Local(local) = stmt {
                        var_decls.push((stmt_idx as usize, local));
                    }
                    if let Stmt::Expr(expr_stmt, _) = stmt {
                        self.collect_var_uses(&expr_stmt, &mut var_uses);
                    }
                }
            }
            if yield_present && var_decls.len() > 0 {
                for (idx, local) in var_decls {
                    if let Pat::Ident(pat_ident) = &local.pat {
                        let var_name = pat_ident.ident.to_string();
                        if var_uses.contains(&var_name) {
                            if let Expr::Block(block) = &*closure.body {
                                self.yield_insertions.push((Box::new(block.clone()), idx));
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl GeneratorVisitor {
    fn collect_var_uses(&self, expr: &Expr, uses: &mut HashSet<String>) {
        match expr {
            Expr::Path(ExprPath { path, .. }) => {
                if path.segments.len() == 1 {
                    uses.insert(path.segments[0].ident.to_string());
                }
            }
            Expr::Call(ExprCall { func, args, .. }) => {
                self.collect_var_uses(func, uses);
                for arg in args {
                    self.collect_var_uses(arg, uses);
                }
            }
            Expr::Assign(ExprAssign { left, right, .. }) => {
                self.collect_var_uses(left, uses);
                self.collect_var_uses(right, uses);
            }
            Expr::Binary(ExprBinary { left, right, .. }) => {
                self.collect_var_uses(left, uses);
                self.collect_var_uses(right, uses);
            }
            Expr::Unary(ExprUnary { expr, .. }) => {
                self.collect_var_uses(expr, uses);
            }
            Expr::Lit(_) => {}
            Expr::Block(ExprBlock { block, .. }) => {
                for stmt in &block.stmts {
                    if let Stmt::Expr(expr_stmt, _) = stmt {
                        self.collect_var_uses(expr_stmt, uses);
                    }
                }
            }
            _ => {}
        }
    }
}