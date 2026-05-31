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
                if let Item::Stmt(s) = item {
                    if s == &leaked_ref_stmt {
                        new_items.push(Item::Stmt(s.clone()));
                        new_items.push(Item::Stmt(deref_stmt));
                    } else if s == &async_move_stmt {
                        let mut closure_visitor = ClosureCaptureReplacer {
                            target: leaked_ref_ident.clone(),
                            replacement: deref_copy_ident.clone(),
                        };
                        closure_visitor.visit_stmt_mut(s);
                        new_items.push(Item::Stmt(s.clone()));
                    } else {
                        new_items.push(item.clone());
                    }
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
                            if let Expr::Call(inner_call) = &*call.args.first().unwrap() {
                                if let Expr::Path(inner_path) = &*inner_call.func {
                                    if inner_path.path.segments.last().map(|s| s.ident.to_string()) == Some("new".to_string()) {
                                        let leaked_ref_stmt = stmt.clone();
                                        let mut async_move_stmt = None;
                                        let mut next_stmt_iter = std::iter::once(stmt.clone());
                                        for next_stmt in next_stmt_iter {
                                            if let Stmt::Expr(expr_stmt, _) = &next_stmt {
                                                if let Expr::Async(async_expr) = expr_stmt {
                                                    if async_expr.capture == syn::AsyncCapture::Move {
                                                        async_move_stmt = Some(next_stmt.clone());
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