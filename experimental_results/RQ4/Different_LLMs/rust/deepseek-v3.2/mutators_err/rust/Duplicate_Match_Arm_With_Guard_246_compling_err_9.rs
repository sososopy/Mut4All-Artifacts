use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    Arm, Block, Expr, ExprBlock, ExprMatch, Pat, PatIdent, PatOr, PatSlice, PatStruct, PatTuple, PatTupleStruct,
    BoundLifetimes, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, PatType, Path as SynPath,
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

pub struct Duplicate_Match_Arm_With_Guard_246;

impl Mutator for Duplicate_Match_Arm_With_Guard_246 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arm_With_Guard_246"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DuplicateMatchArmVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates match arms that have both a pattern binding variables and a guard condition. For each such arm, it creates an identical duplicate immediately after it, using the same pattern and guard. The duplicate's body is replaced with an empty block `{}` to avoid potential side effects or invalid code from duplication. This transformation tests the compiler's handling of redundant match arms, guard evaluation, and pattern binding consistency, potentially exposing issues in exhaustiveness checking, control flow, or variable shadowing."
    }
}

struct DuplicateMatchArmVisitor;

impl VisitMut for DuplicateMatchArmVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        let mut new_arms = Vec::new();
        let mut i = -1;
        let arms_vec: Vec<_> = expr.arms.iter().collect();
        while i < arms_vec.len() as isize - 1 {
            i += 1;
            let arm = arms_vec[i as usize];
            new_arms.push(arm.clone());
            
            if let Some((guard, pattern_binds_vars)) = has_guard_and_binding(&arm) {
                if !has_consecutive_duplicate(&expr.arms, i as usize) {
                    let mut duplicate_arm = arm.clone();
                    duplicate_arm.body = Box::new(Expr::Block(ExprBlock {
                        attrs: Vec::new(),
                        label: None,
                        block: Block {
                            brace_token: token::Brace::default(),
                            stmts: Vec::new(),
                        },
                    }));
                    new_arms.push(duplicate_arm);
                }
            }
        }
        expr.arms = Punctuated::from_iter(new_arms);
        visit_mut::visit_expr_match_mut(self, expr);
    }
}

fn has_guard_and_binding(arm: &Arm) -> Option<(Option<&syn::Expr>, bool)> {
    let guard = arm.guard.as_ref().map(|(_, expr)| &**expr);
    let pattern_binds_vars = binds_variables(&arm.pat);
    if pattern_binds_vars {
        Some((guard, pattern_binds_vars))
    } else {
        None
    }
}

fn binds_variables(pat: &Pat) -> bool {
    struct BindingVisitor(bool);
    impl Visit<'_> for BindingVisitor {
        fn visit_pat_ident(&mut self, _: &PatIdent) {
            self.0 = true;
        }
        fn visit_pat_struct(&mut self, i: &PatStruct) {
            for field in &i.fields {
                self.visit_pat(&field.pat);
            }
        }
        fn visit_pat_tuple_struct(&mut self, i: &PatTupleStruct) {
            for elem in &i.elems {
                self.visit_pat(elem);
            }
        }
        fn visit_pat_tuple(&mut self, i: &PatTuple) {
            for elem in &i.elems {
                self.visit_pat(elem);
            }
        }
        fn visit_pat_slice(&mut self, i: &PatSlice) {
            for elem in &i.elems {
                self.visit_pat(elem);
            }
        }
        fn visit_pat_or(&mut self, i: &PatOr) {
            for case in &i.cases {
                self.visit_pat(case);
            }
        }
    }
    let mut visitor = BindingVisitor(false);
    visitor.visit_pat(pat);
    visitor.0
}

fn has_consecutive_duplicate(arms: &Punctuated<Arm, Comma>, index: usize) -> bool {
    if index + 1 >= arms.len() {
        return false;
    }
    let arm1 = &arms[index];
    let arm2 = &arms[index + 1];
    patterns_equal(&arm1.pat, &arm2.pat) && guards_equal(&arm1.guard.as_ref().map(|(_, expr)| &**expr), &arm2.guard.as_ref().map(|(_, expr)| &**expr))
}

fn patterns_equal(pat1: &Pat, pat2: &Pat) -> bool {
    quote!(#pat1).to_string() == quote!(#pat2).to_string()
}

fn guards_equal(guard1: &Option<&syn::Expr>, guard2: &Option<&syn::Expr>) -> bool {
    match (guard1, guard2) {
        (Some(g1), Some(g2)) => quote!(#g1).to_string() == quote!(#g2).to_string(),
        (None, None) => true,
        _ => false,
    }
}