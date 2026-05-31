use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatIdent, PatType, Path as SynPath,
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

pub struct Capture_Self_By_Value_37;

impl Mutator for Capture_Self_By_Value_37 {
    fn name(&self) -> &str {
        "Capture_Self_By_Value_37"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let mut call_sites_to_modify = Vec::new();
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                let var_name = get_local_var_name(local);
                                if let Some(init) = &mut local.init {
                                    if let Expr::Closure(closure) = &mut *init.expr {
                                        let uses_self = {
                                            let mut visitor = SelfUsageVisitor::new();
                                            visitor.visit_expr(&closure.body);
                                            visitor.found
                                        };
                                        if uses_self {
                                            add_self_param_to_closure(closure);
                                            replace_self_with_inner_self(closure);
                                            call_sites_to_modify.push(var_name);
                                        }
                                    }
                                }
                            }
                        }
                        for var_name in call_sites_to_modify {
                            let mut visitor = CallSiteVisitor::new(var_name);
                            visitor.visit_block_mut(&mut func.block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms closures that capture `self` by mutable reference into closures that take `self` by value. This introduces ownership transfer, creating potential ICEs when the outer scope attempts to use `self` after the closure's invocation. The transformation modifies both the closure's parameters and all call sites to pass `self.clone()`, ensuring the closure takes ownership while the outer scope may still attempt to use the moved value."
    }
}

struct SelfUsageVisitor {
    found: bool,
}

impl SelfUsageVisitor {
    fn new() -> Self {
        Self { found: false }
    }
}

impl Visit<'_> for SelfUsageVisitor {
    fn visit_expr_path(&mut self, expr_path: &ExprPath) {
        if let Some(segment) = expr_path.path.segments.iter().next() {
            if segment.ident == "self" {
                self.found = true;
            }
        }
        syn::visit::visit_expr_path(self, expr_path);
    }
}

fn get_local_var_name(local: &Local) -> Ident {
    if let Pat::Ident(pat_ident) = &local.pat {
        pat_ident.ident.clone()
    } else {
        Ident::new("unknown", Span::call_site())
    }
}

fn add_self_param_to_closure(closure: &mut ExprClosure) {
    let new_param = PatType {
        attrs: vec![],
        pat: Box::new(Pat::Ident(PatIdent {
            attrs: vec![],
            by_ref: None,
            mutability: Some(syn::token::Mut::ne(Span::call_site())),
            ident: Ident::new("inner_self", Span::call_site()),
            subpat: None,
        })),
        colon_token: Some(syn::token::Colon::ne(Span::call_site())),
        ty: Box::new(Type::Path(TypePath {
            qself: None,
            path: parse_quote!(Self),
        })),
    };
    closure.inputs.push(Pat::Type(new_param));
}

fn replace_self_with_inner_self(closure: &mut ExprClosure) {
    let new_ident = Ident::new("inner_self", Span::call_site());
    let mut visitor = ReplaceSelfVisitor { new_ident };
    visitor.visit_expr_mut(&mut closure.body);
}

struct ReplaceSelfVisitor {
    new_ident: Ident,
}

impl VisitMut for ReplaceSelfVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if let Some(segment) = expr_path.path.segments.iter().next() {
                if segment.ident == "self" {
                    *expr = Expr::Path(syn::ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: {
                            let mut segs = Punctuated::new();
                            segs.push(PathSegment {
                                ident: self.new_ident.clone(),
                                arguments: PathArguments::None,
                            });
                            SynPath {
                                leading_colon: None,
                                segments: segs,
                            }
                        },
                    });
                    return;
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}

struct CallSiteVisitor {
    var_name: Ident,
}

impl CallSiteVisitor {
    fn new(var_name: Ident) -> Self {
        Self { var_name }
    }

    fn modify_call_expr(&self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                if let Some(segment) = path_expr.path.segments.iter().next() {
                    if segment.ident == self.var_name {
                        let new_arg = parse_quote!(self.clone());
                        let mut args = call_expr.args.clone();
                        args.insert(0, new_arg);
                        call_expr.args = args;
                    }
                }
            }
        }
    }
}

impl VisitMut for CallSiteVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        self.modify_call_expr(expr);
        visit_mut::visit_expr_mut(self, expr);
    }
}