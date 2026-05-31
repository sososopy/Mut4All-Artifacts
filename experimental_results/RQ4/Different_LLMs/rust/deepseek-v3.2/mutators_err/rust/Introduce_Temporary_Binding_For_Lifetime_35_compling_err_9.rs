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

pub struct Introduce_Temporary_Binding_For_Lifetime_35;

impl Mutator for Introduce_Temporary_Binding_For_Lifetime_35 {
    fn name(&self) -> &str {
        "Introduce_Temporary_Binding_For_Lifetime_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, ty) = &func.sig.output {
                    if let Type::ImplTrait(_) = &**ty {
                        let mut visitor = AsyncBlockVisitor::new();
                        visitor.visit_block_mut(&mut func.block);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item_fn in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item_fn {
                        if let ReturnType::Type(_, ty) = &func.sig.output {
                            if let Type::ImplTrait(_) = &**ty {
                                let mut visitor = AsyncBlockVisitor::new();
                                visitor.visit_block_mut(&mut func.block);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return an opaque future type (e.g., `impl Future<Output = T>`) and contain async blocks. It applies when there is a reference inside the async block that is used in a function call that expects a reference with a specific lifetime, particularly when that function call involves another future that may capture the reference. The mutation inserts a new `let` binding that holds the reference, then uses that binding in the original call. This can alter lifetime inference by introducing an additional temporary scope."
    }
}

struct AsyncBlockVisitor {
    temp_binding_counter: usize,
    parent_block: Option<*mut syn::Block>,
}

impl AsyncBlockVisitor {
    fn new() -> Self {
        Self { temp_binding_counter: 0, parent_block: None }
    }
}

impl VisitMut for AsyncBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Async(expr_async) = expr {
            self.visit_block_mut(&mut expr_async.block);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        let previous_parent = self.parent_block;
        self.parent_block = Some(block as *mut syn::Block);
        syn::visit_mut::visit_block_mut(self, block);
        self.parent_block = previous_parent;
    }

    fn visit_stmt_mut(&mut self, stmt: &mut syn::Stmt) {
        let stmt_ptr = stmt as *mut syn::Stmt;
        if let syn::Stmt::Expr(expr_stmt, _) = stmt {
            if let syn::Expr::Call(expr_call) = expr_stmt {
                if let syn::Expr::Path(expr_path) = &*expr_call.func {
                    let func_name = expr_path.path.segments.last().map(|seg| seg.ident.to_string());
                    if let Some(name) = func_name {
                        if !name.starts_with("std::") && !name.starts_with("core::") {
                            for arg in &mut expr_call.args {
                                if let syn::Expr::Reference(expr_ref) = arg {
                                    self.temp_binding_counter += 1;
                                    let temp_binding_name = format!("temp_ref_{}", self.temp_binding_counter);
                                    let temp_ident = syn::Ident::new(&temp_binding_name, Span::call_site());
                                    let new_stmt = syn::Stmt::Local(syn::Local {
                                        attrs: Vec::new(),
                                        let_token: syn::token::Let::default(),
                                        pat: syn::Pat::Ident(syn::PatIdent {
                                            attrs: Vec::new(),
                                            ident: temp_ident.clone(),
                                            by_ref: None,
                                            mutability: None,
                                            subpat: None,
                                        }),
                                        init: Some(syn::LocalInit {
                                            eq_token: syn::token::Eq::default(),
                                            expr: Box::new(syn::Expr::Reference(syn::ExprReference {
                                                attrs: Vec::new(),
                                                and_token: syn::token::And::default(),
                                                mutability: None,
                                                expr: Box::new(*expr_ref.expr.clone()),
                                            })),
                                            diverge: None,
                                        }),
                                        semi_token: syn::token::Semi::default(),
                                    });
                                    if let Some(parent_block_ptr) = self.parent_block {
                                        unsafe {
                                            let parent_block = &mut *parent_block_ptr;
                                            let stmt_index = parent_block.stmts.iter().position(|s| std::ptr::eq(s, stmt_ptr));
                                            if let Some(index) = stmt_index {
                                                parent_block.stmts.insert(index, new_stmt);
                                            }
                                        }
                                    }
                                    *arg = syn::Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: syn::Path::from(temp_ident),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}