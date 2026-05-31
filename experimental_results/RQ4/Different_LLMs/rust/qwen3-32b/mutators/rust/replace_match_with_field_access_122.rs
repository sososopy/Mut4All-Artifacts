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

pub struct Replace_Match_With_Field_Access_122;

impl Mutator for Replace_Match_With_Field_Access_122 {
    fn name(&self) -> &str {
        "Replace_Match_With_Field_Access_122"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ReplaceMatchVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces `match` expressions on context structs with direct field accesses (e.g., `let _ = &ctx.field;`). This transformation alters how generators interact with type aliases and associated types by changing the syntactic structure while preserving dependencies. It encourages the compiler to explore alternative type-checking and codegen paths, potentially exposing ICEs in generator-related type inference or trait resolution."
    }
}

struct ReplaceMatchVisitor;

impl VisitMut for ReplaceMatchVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Match(expr_match) = expr {
            if let Expr::Path(expr_path) = &*expr_match.expr {
                if let Some(var_name) = expr_path.path.get_ident() {
                    let new_expr = parse_quote! { { let _ = & #var_name.field; } };
                    *expr = new_expr;
                } else {
                    syn::visit_mut::visit_expr_mut(self, expr);
                }
            } else {
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}