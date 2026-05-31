use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Mutate_Pattern_Matching_Ref_Mut_248;

impl Mutator for Mutate_Pattern_Matching_Ref_Mut_248 {
    fn name(&self) -> &str {
        "Mutate_Pattern_Matching_Ref_Mut_248"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = PatternMatcherVisitor::default();
                visitor.visit_item_fn_mut(func);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets pattern matching expressions, specifically `if let` and `match`, altering reference patterns to mutable references or vice versa. This transformation tests the compiler's handling of pattern mutability and reference semantics, potentially revealing issues in the type system or borrow checker."
    }
}

#[derive(Default)]
struct PatternMatcherVisitor;

impl VisitMut for PatternMatcherVisitor {
    fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
        for arm in &mut expr_match.arms {
            if let Pat::Reference(pat_ref) = &mut arm.pat {
                if pat_ref.mutability.is_some() {
                    pat_ref.mutability = None;
                } else {
                    pat_ref.mutability = Some(token::Mut { span: Span::call_site() });
                }
            }
        }
        syn::visit_mut::visit_expr_match_mut(self, expr_match);
    }

    fn visit_expr_if_mut(&mut self, expr_if: &mut ExprIf) {
        if let Some((_, expr)) = &mut expr_if.cond {
            if let Expr::Let(expr_let) = &mut **expr {
                if let Pat::Reference(pat_ref) = &mut expr_let.pat {
                    if pat_ref.mutability.is_some() {
                        pat_ref.mutability = None;
                    } else {
                        pat_ref.mutability = Some(token::Mut { span: Span::call_site() });
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_if_mut(self, expr_if);
    }
}