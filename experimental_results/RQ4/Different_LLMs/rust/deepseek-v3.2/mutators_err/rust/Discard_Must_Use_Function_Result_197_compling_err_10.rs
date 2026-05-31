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

pub struct Discard_Must_Use_Function_Result_197;

impl Mutator for Discard_Must_Use_Function_Result_197 {
    fn name(&self) -> &str {
        "Discard_Must_Use_Function_Result_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut must_use_functions = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.attrs.iter().any(|attr| attr.path().is_ident("must_use")) {
                    must_use_functions.push(item_fn.sig.ident.clone());
                }
            }
        }
        if must_use_functions.is_empty() {
            return;
        }
        let mut visitor = DiscardMustUseVisitor {
            target_functions: must_use_functions,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function call expressions where the called function has a #[must_use] attribute. It modifies the call site to explicitly discard the result using a let _ = ...; statement or an underscore assignment _ = ...;. This transformation can alter the diagnostic emission flow for must_use lints, potentially exposing bugs in the compiler's diagnostic tracking, particularly around trimmed_def_paths and ICE conditions."
    }
}

struct DiscardMustUseVisitor {
    target_functions: Vec<Ident>,
    mutated: bool,
}

impl VisitMut for DiscardMustUseVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                if let Some(segment) = path_expr.path.segments.last() {
                    if self.target_functions.contains(&segment.ident) {
                        let new_stmt: Stmt = parse_quote! { let _ = #call_expr; };
                        *expr = Expr::Verbatim(quote::quote! { #new_stmt });
                        self.mutated = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Expr(expr_stmt, _) = stmt {
            if let Expr::Call(call_expr) = expr_stmt {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    if let Some(segment) = path_expr.path.segments.last() {
                        if self.target_functions.contains(&segment.ident) {
                            let new_stmt: Stmt = parse_quote! { _ = #call_expr; };
                            *stmt = Stmt::Expr(Expr::Verbatim(quote::quote! { #new_stmt }), None);
                            self.mutated = true;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}