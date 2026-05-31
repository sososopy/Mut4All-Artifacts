use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprAsync, ExprBlock, ExprClosure, File, FnArg, GenericArgument,
    GenericParam, Ident, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
    TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned, token, token::Comma,
    token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Replace_Closure_With_Async_In_Const_Expr_373;

impl Mutator for Replace_Closure_With_Async_In_Const_Expr_373 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Async_In_Const_Expr_373"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ReplaceClosureInConstVisitor;
                visitor.visit_item_fn_mut(func);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let mut visitor = ReplaceClosureInConstVisitor;
                        visitor.visit_impl_item_fn_mut(method);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces closures in const expressions (e.g., array lengths) with async blocks, creating type mismatches. This tests the compiler's handling of async blocks in const contexts and closure type inference within const expressions."
    }
}

struct ReplaceClosureInConstVisitor;

impl<'ast> VisitMut for ReplaceClosureInConstVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(array_type) = ty {
            let mut async_replacer = ReplaceClosureWithAsync;
            async_replacer.visit_expr_mut(&mut array_type.len);
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

struct ReplaceClosureWithAsync;

impl<'ast> VisitMut for ReplaceClosureWithAsync {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(_) = expr {
            *expr = parse_quote! { async { 1 } };
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}