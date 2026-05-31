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

pub struct Force_Deref_In_Const_Context_328;

impl Mutator for Force_Deref_In_Const_Context_328 {
    fn name(&self) -> &str {
        "Force_Deref_In_Const_Context_328"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut deref_exprs = Vec::new();
        let mut visitor = DerefVisitor { exprs: &mut deref_exprs };
        visitor.visit_file(file);

        if deref_exprs.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let selected_expr = deref_exprs.choose(&mut rng).unwrap();
        let span = selected_expr.span();

        let const_item: Item = parse_quote! {
            const _: () = {
                let _deref_check = #selected_expr;
            };
        };

        file.items.push(const_item);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies dereference expressions in the seed program and forces them into a const context by wrapping them in a const item. This transformation requires the compiler to verify const trait bounds (like const Deref) during type checking and trait solving in const contexts. It aims to expose bugs where the compiler incorrectly handles or delays errors related to const trait implementations for dereferencing operations."
    }
}

struct DerefVisitor<'a> {
    exprs: &'a mut Vec<Box<Expr>>,
}

impl<'a> Visit<'a> for DerefVisitor<'a> {
    fn visit_expr(&mut self, expr: &'a Expr) {
        match expr {
            Expr::Unary(unary_expr) => {
                if unary_expr.op == syn::UnOp::Deref(syn::token::Star::default()) {
                    self.exprs.push(Box::new(expr.clone()));
                }
            }
            Expr::MethodCall(method_call) => {
                let method_name = method_call.method.to_string();
                if method_name == "len" || method_name == "as_str" || method_name == "as_ref" {
                    self.exprs.push(Box::new(expr.clone()));
                }
            }
            _ => {}
        }
        syn::visit::visit_expr(self, expr);
    }
}