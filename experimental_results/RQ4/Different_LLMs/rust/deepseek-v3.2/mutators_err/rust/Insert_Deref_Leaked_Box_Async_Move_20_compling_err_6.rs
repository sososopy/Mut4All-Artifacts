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

pub struct Insert_Deref_Leaked_Box_Async_Move_20;

impl Mutator for Insert_Deref_Leaked_Box_Async_Move_20 {
    fn name(&self) -> &str {
        "Insert_Deref_Leaked_Box_Async_Move_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = LeakedBoxVisitor {
            mutations: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for mutation in visitor.mutations {
            let (leaked_ref_stmt, async_move_stmt) = mutation;
            let leaked_ref_ident = extract_ident_from_stmt(&leaked_ref_stmt);
            if leaked_ref_ident.is_none() {
                continue;
            }
            let leaked_ref_ident = leaked_ref_ident.unwrap();
            let deref_copy_ident = Ident::new("deref_copy", Span::call_site());
            let deref_stmt: Stmt = parse_quote! {
                let deref_copy = *leaked_ref_ident;
            };
            let mut new_items = Vec::new();
            for item in &mut file.items {
                if let Item::Fn(fn_item) = item {
                    let mut new_stmts = Vec::new();
                    for stmt in &fn_item.block.stmts {
                        if stmt == &leaked_ref_stmt {
                            new_stmts.push(stmt.clone());
                            new_stmts.push(deref_stmt.clone());
                        } else if stmt == &async_move_stmt {
                            let mut closure_visitor = ClosureCaptureReplacer {
                                target: leaked_ref_ident.clone(),
                                replacement: deref_copy_ident.clone(),
                            };
                            let mut stmt_clone = stmt.clone();
                            closure_visitor.visit_stmt_mut(&mut stmt_clone);
                            new_stmts.push(stmt_clone);
                        } else {
                            new_stmts.push(stmt.clone());
                        }
                    }
                    let mut new_fn = fn_item.clone();
                    new_fn.block.stmts = new_stmts;
                    new_items.push(Item::Fn(new_fn));
                } else {
                    new_items.push(item.clone());
                }
            }
            file.items = new_items;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async move closures that capture a reference obtained via Box::leak. It inserts an explicit dereference of the leaked pointer before the closure, creating a new variable deref_copy that holds the dereferenced value. The closure then captures deref_copy instead of the original leaked reference. This transformation can alter inferred types and borrow interactions, potentially exposing lifetime normalization or type equating bugs in async contexts, as seen in bug reports where capturing a leaked Box reference in an async move closure triggered an ICE."
    }
}

struct LeakedBoxVisitor {
    mutations: Vec<(Stmt, Stmt)>,
}

impl VisitMut for LeakedBoxVisitor {
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Local(local) = stmt {
            if let Some(init) = &local.init {
                if let Expr::Call(call) = &*init.expr {
                    if let Expr::Path(path) = &*call.func {
                        if path.path.segments.last().map(|s| s.ident.to_string()) == Some("leak".to_string()) {
                            if let Some(first_arg) = call.args.first() {
                                if let Expr::Call(inner_call) = &*first_arg {
                                    if let Expr::Path(inner_path) = &*inner_call.func {
                                        if inner_path.path.segments.last().map(|s| s.ident.to_string()) == Some("new".to_string()) {
                                            let leaked_ref_stmt = stmt.clone();
                                            let mut async_move_stmt = None;
                                            let mut found = false;
                                            let mut current_block: Option<syn::Block> = None;
                                            syn::visit_mut::visit_stmt_mut(self, stmt);
                                            if let Some(block) = current_block {
                                                for next_stmt in &block.stmts {
                                                    if let Stmt::Expr(expr_stmt, _) = next_stmt {
                                                        if let Expr::Async(async_expr) = expr_stmt {
                                                            if let syn::ExprAsync { movability: syn::Movability::Static, .. } = async_expr {
                                                                async_move_stmt = Some(next_stmt.clone());
                                                                found = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            if found {
                                                if let Some(async_move_stmt) = async_move_stmt {
                                                    self.mutations.push((leaked_ref_stmt, async_move_stmt));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        let stmts = block.stmts.clone();
        for (i, stmt) in stmts.iter().enumerate() {
            if let Stmt::Local(local) = stmt {
                if let Some(init) = &local.init {
                    if let Expr::Call(call) = &*init.expr {
                        if let Expr::Path(path) = &*call.func {
                            if path.path.segments.last().map(|s| s.ident.to_string()) == Some("leak".to_string()) {
                                if let Some(first_arg) = call.args.first() {
                                    if let Expr::Call(inner_call) = &*first_arg {
                                        if let Expr::Path(inner_path) = &*inner_call.func {
                                            if inner_path.path.segments.last().map(|s| s.ident.to_string()) == Some("new".to_string()) {
                                                let leaked_ref_stmt = stmt.clone();
                                                let mut async_move_stmt = None;
                                                for j in i+1..stmts.len() {
                                                    if let Stmt::Expr(expr_stmt, _) = &stmts[j] {
                                                        if let Expr::Async(async_expr) = expr_stmt {
                                                            if let syn::ExprAsync { movability: syn::Movability::Static, .. } = async_expr {
                                                                async_move_stmt = Some(stmts[j].clone());
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                                if let Some(async_move_stmt) = async_move_stmt {
                                                    self.mutations.push((leaked_ref_stmt, async_move_stmt));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_block_mut(self, block);
    }
}

fn extract_ident_from_stmt(stmt: &Stmt) -> Option<Ident> {
    if let Stmt::Local(local) = stmt {
        if let Pat::Ident(pat_ident) = &local.pat {
            return Some(pat_ident.ident.clone());
        }
    }
    None
}

struct ClosureCaptureReplacer {
    target: Ident,
    replacement: Ident,
}

impl VisitMut for ClosureCaptureReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(path) = expr {
            if path.path.segments.len() == 1 && path.path.segments[0].ident == self.target {
                path.path.segments[0].ident = self.replacement.clone();
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}