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

pub struct RefCell_BorrowExtender_126;

impl Mutator for RefCell_BorrowExtender_126 {
    fn name(&self) -> &str {
        "RefCell_BorrowExtender_126"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = BorrowExtenderVisitor::new();
                visitor.visit_block_mut(&mut func.block);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let mut visitor = BorrowExtenderVisitor::new();
                        visitor.visit_block_mut(&mut method.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies if-let patterns that directly dereference a mutable borrow of a RefCell with a destructor. It introduces a named variable to capture the borrowed reference, extending its lifetime and increasing the likelihood of exposing borrow checker errors related to destructor execution and extended borrows. This stresses NLL analysis by requiring tracking of the borrow across control flow paths."
    }
}

struct BorrowExtenderVisitor;

impl BorrowExtenderVisitor {
    fn new() -> Self {
        Self {}
    }
}

impl VisitMut for BorrowExtenderVisitor {
    fn visit_block_mut(&mut self, block: &mut syn::Block) {
        for i in (0..block.stmts.len()).rev() {
            if let Stmt::Expr(expr, _) = &mut block.stmts[i] {
                if let Expr::If(expr_if) = &mut **expr {
                    if let Expr::Let(expr_let) = &mut *expr_if.cond {
                        if let Expr::Unary(expr_unary) = &mut *expr_let.expr {
                            if let UnOp::Deref(_) = expr_unary.op {
                                if let Expr::MethodCall(method_call) = &mut *expr_unary.expr {
                                    if method_call.method == "borrow_mut" {
                                        let var_name = Ident::new("mut_borrow", Span::call_site());
                                        let new_local = Local {
                                            attrs: vec![],
                                            let_token: Default::default(),
                                            pat: Pat::Ident(syn::PatIdent {
                                                attrs: vec![],
                                                ident: var_name.clone(),
                                                subpat: None,
                                                mutability: None,
                                                by_ref: None,
                                            }),
                                            init: Some(syn::LocalInit {
                                                eq_token: Default::default(),
                                                expr: Box::new(method_call.clone()),
                                            }),
                                            semi_token: Default::default(),
                                        };
                                        block.stmts.insert(i, Stmt::Local(new_local));
                                        expr_let.expr = Box::new(Expr::Unary(ExprUnary {
                                            attrs: vec![],
                                            op: UnOp::Deref(Default::default()),
                                            expr: Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: SynPath::from(var_name),
                                            })),
                                        }));
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