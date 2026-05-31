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

pub struct Replace_Mutable_Borrow_Deref_In_Let_Guard_126;

impl Mutator for Replace_Mutable_Borrow_Deref_In_Let_Guard_126 {
    fn name(&self) -> &str {
        "Replace_Mutable_Borrow_Deref_In_Let_Guard_126"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MutVisitor {
            mutable_methods: HashSet::new(),
            immutable_methods: HashSet::new(),
        };
        visitor.visit_file(file);
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = MutVisitor {
                    mutable_methods: visitor.mutable_methods.clone(),
                    immutable_methods: visitor.immutable_methods.clone(),
                };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies `if let` or `while let` expressions where the right-hand side is a dereference of a mutable borrow method call (e.g., `borrow_mut()`). It replaces the mutable method with its immutable counterpart (e.g., `borrow()`) if such a method exists in the seed's context. This creates a type mismatch because the pattern expects ownership or a mutable reference but receives an immutable reference, potentially triggering lifetime or type system bugs related to destructors and NLL."
    }
}

struct MutVisitor {
    mutable_methods: HashSet<String>,
    immutable_methods: HashSet<String>,
}

impl Visit<'_> for MutVisitor {
    fn visit_expr(&mut self, expr: &Expr) {
        if let Expr::MethodCall(method_call) = expr {
            let method_name = method_call.method.to_string();
            if method_name.ends_with("_mut") {
                self.mutable_methods.insert(method_name.clone());
                let immutable_name = method_name[..method_name.len() - 4].to_string();
                self.immutable_methods.insert(immutable_name);
            }
        }
        syn::visit::visit_expr(self, expr);
    }
}

impl VisitMut for MutVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::If(expr_if) = expr {
            if let Expr::Let(expr_let) = &mut *expr_if.cond {
                self.process_let_expr(expr_let);
            }
        } else if let Expr::While(expr_while) = expr {
            if let Expr::Let(expr_2let) = &mut *expr_while.cond {
                self.process_2let_expr(expr_2let);
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_item_fn_mut(&mut self, item_fn: &mut ItemFn) {
        syn::visit_mut::visit_item_fn_mut(self, item_fn);
    }
}

impl MutVisitor {
    fn process_let_expr(&mut self, expr_let: &mut ExprLet) {
        if let Expr::Unary(unary) = &*expr_let.expr {
            if let UnOp::Deref(_) = unary.op {
                if let Expr::MethodCall(method_call) = &*unary.expr {
                    let method_name = method_call.method.to_string();
                    if self.mutable_methods.contains(&method_name) && method_name.ends_with("_mut") {
                        let immutable_name = method_name[..method_name.len() - 4].to_string();
                        if self.immutable_methods.contains(&immutable_name) {
                            let new_method = Ident::new(&immutable_name, method_call.method.span());
                            let mut new_call = method_call.clone();
                            new_call.method = new_method;
                            let new_unary = ExprUnary {
                                attrs: unary.attrs.clone(),
                                op: UnOp::Deref(token::Star::default()),
                                expr: Box::new(Expr::MethodCall(new_call)),
                            };
                            *expr_let.expr = Expr::Unary(new_unary);
                        }
                    }
                }
            }
        }
    }
    fn process_2let_expr(&mut self, expr_2let: &mut ExprLet) {
        if let Expr::Unary(unary) = &*expr_2let.expr {
            if let UnOp::Deref(_) = unary.op {
                if let Expr::MethodCall(method_call) = &*unary.expr {
                    let method_name = method_call.method.to_string();
                    if self.mutable_methods.contains(&method_name) && method_name.ends_with("_mut") {
                        let immutable_name = method_name[..method_name.len() - 4].to_string();
                        if self.immutable_methods.contains(&immutable_name) {
                            let new_method = Ident::new(&immutable_name, method_call.method.span());
                            let mut new_call = method_call.clone();
                            new_call.method = new_method;
                            let new_unary = ExprUnary {
                                attrs: unary.attrs.clone(),
                                op: UnOp::Deref(token::Star::default()),
                                expr: Box::new(Expr::MethodCall(new_call)),
                            };
                            *expr_2let.expr = Expr::Unary(new_unary);
                        }
                    }
                }
            }
        }
    }
}