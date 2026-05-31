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

pub struct Nested_Reference_Pattern_Expansion_67;

impl Mutator for Nested_Reference_Pattern_Expansion_67 {
    fn name(&self) -> &str {
        "Nested_Reference_Pattern_Expansion_67"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MatchReferenceExpander {};
        syn::visit_mut::visit_file_mut(&mut visitor, file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions where the scrutinee is a reference and the pattern is a reference to a constant. It increases the reference nesting in both the scrutinee and the pattern, forcing the compiler to evaluate the constant in a deeper reference context. This tests const evaluation paths that mishandle reference layers in patterns, particularly when the constant's type involves non-structural equality or requires dynamic evaluation."
    }
}

struct MatchReferenceExpander;

impl VisitMut for MatchReferenceExpander {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        if let Expr::Reference(ref_expr) = &mut *expr.expr {
            let new_ref_expr = Expr::Reference(ExprReference {
                attrs: Vec::new(),
                and_token: token::And::default(),
                mutability: None,
                expr: Box::new(Expr::Reference(ref_expr.clone())),
            });
            expr.expr = Box::new(new_ref_expr);
        }

        for arm in &mut expr.arms {
            if let &mut Pat::Reference(ref mut ref_pat) = &mut **arm.pat {
                if let Pat::Ident(_) = &*ref_pat.pat {
                    let inner_pat = std::mem::replace(&mut ref_pat.pat, Box::new(Pat::Wild(PatWild {
                        attrs: Vec::new(),
                        underscore_token: Default::default(),
                    })));
                    let new_inner_ref = Pat::Reference(PatReference {
                        attrs: Vec::new(),
                        and_token: token::And::default(),
                        mutability: None,
                        pat: inner_pat,
                    });
                    arm.pat = Box::new(new_inner_ref);
                }
            }
        }

        syn::visit_mut::visit_expr_match_mut(self, expr);
    }
}