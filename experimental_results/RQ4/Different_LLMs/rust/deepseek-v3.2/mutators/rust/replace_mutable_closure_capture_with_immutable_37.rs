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

pub struct Replace_Mutable_Closure_Capture_With_Immutable_37;

impl Mutator for Replace_Mutable_Closure_Capture_With_Immutable_37 {
    fn name(&self) -> &str {
        "Replace_Mutable_Closure_Capture_With_Immutable_37"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor {
            mutations: Vec<(Box<Expr>, Box<Expr>, Vec<Stmt>)>,
        }
        impl VisitMut for ClosureVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Closure(closure) = expr {
                    let mut found_mut_self = false;
                    let mut mutation_stmts = Vec::new();
                    let mut other_stmts = Vec::new();
                    if let Expr::Block(block) = &*closure.body {
                        for stmt in &block.block.stmts {
                            if let Stmt::Expr(expr_stmt, _) = stmt {
                                if let Expr::Assign(assign) = expr_stmt {
                                    if let Expr::Path(path) = &*assign.left {
                                        if path.path.segments.len() == 2 && path.path.segments[0].ident == "self" {
                                            found_mut_self = true;
                                            mutation_stmts.push(stmt.clone());
                                        } else {
                                            other_stmts.push(stmt.clone());
                                        }
                                    } else {
                                        other_stmts.push(stmt.clone());
                                    }
                                } else {
                                    other_stmts.push(stmt.clone());
                                }
                            } else {
                                other_stmts.push(stmt.clone());
                            }
                        }
                    }
                    if found_mut_self && mutation_stmts.len() > 0 {
                        let mut new_closure = closure.clone();
                        if let Expr::Block(block) = &mut *new_closure.body {
                            block.block.stmts = other_stmts;
                        }
                        self.mutations.push((Box::new(Expr::Closure(new_closure)), Box::new(expr.clone()), mutation_stmts));
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureVisitor { mutations: Vec::new() };
                visitor.visit_block_mut(&mut item_fn.block);
                for (new_closure, old_expr, mutation_stmts) in visitor.mutations {
                    if mutation_stmts.len() > 0 {
                        let mut_ref_stmt: Stmt = parse_quote! { let mut_ref = &mut self; };
                        let mut new_stmts = Vec::new();
                        new_stmts.push(mut_ref_stmt);
                        for stmt in mutation_stmts {
                            if let Stmt::Expr(Expr::Assign(assign), _) = stmt {
                                if let Expr::Path(path) = &*assign.left {
                                    if path.path.segments.len() == 2 && path.path.segments[0].ident == "self" {
                                        let field = &path.path.segments[1].ident;
                                        let value = &assign.right;
                                        let new_stmt: Stmt = parse_quote! { mut_ref.#field = #value; };
                                        new_stmts.push(new_stmt);
                                    }
                                }
                            }
                        }
                        let mut block_stmts = item_fn.block.stmts.clone();
                        for i in 0..block_stmts.len() {
                            if let Stmt::Expr(expr_stmt, _) = &block_stmts[i] {
                                if expr_stmt == &*old_expr {
                                    block_stmts[i] = Stmt::Expr(*new_closure, None);
                                    block_stmts.splice(i..i, new_stmts);
                                    break;
                                }
                            }
                        }
                        item_fn.block.stmts = block_stmts;
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut visitor = ClosureVisitor { mutations: Vec::new() };
                        visitor.visit_block_mut(&mut method.block);
                        for (new_closure, old_expr, mutation_stmts) in visitor.mutations {
                            if mutation_stmts.len() > 0 {
                                let mut_ref_stmt: Stmt = parse_quote! { let mut_ref = &mut self; };
                                let mut new_stmts = Vec::new();
                                new_stmts.push(mut_ref_stmt);
                                for stmt in mutation_stmts {
                                    if let Stmt::Expr(Expr::Assign(assign), _) = stmt {
                                        if let Expr::Path(path) = &*assign.left {
                                            if path.path.segments.len() == 2 && path.path.segments[0].ident == "self" {
                                                let field = &path.path.segments[1].ident;
                                                let value = &assign.right;
                                                let new_stmt: Stmt = parse_quote! { mut_ref.#field = #value; };
                                                new_stmts.push(new_stmt);
                                            }
                                        }
                                    }
                                }
                                let mut block_stmts = method.block.stmts.clone();
                                for i in 0..block_stmts.len() {
                                    if let Stmt::Expr(expr_stmt, _) = &block_stmts[i] {
                                        if expr_stmt == &*old_expr {
                                            block_stmts[i] = Stmt::Expr(*new_closure, None);
                                            block_stmts.splice(i..i, new_stmts);
                                            break;
                                        }
                                    }
                                }
                                method.block.stmts = block_stmts;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies closures within methods that capture self mutably and contain assignments to self fields. It transforms the closure to capture self immutably by removing mutation statements, moves those mutations outside the closure using a mutable reference mut_ref, and reinserts the closure with only non-mutating statements. This creates overlapping mutable and immutable borrows, aiming to trigger borrow checker conflicts and test Rust's lifetime analysis under complex closure capture scenarios."
    }
}