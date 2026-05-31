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

pub struct Duplicate_Match_Arm_With_Single_Arm_246;

struct DuplicateMatchArmVisitor;

impl<'ast> VisitMut for DuplicateMatchArmVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        if expr.arms.len() == 1 {
            let arm = expr.arms[0].clone();
            expr.arms.push(arm);
        }
        syn::visit_mut::visit_expr_match_mut(self, expr);
    }
}

impl Mutator for Duplicate_Match_Arm_With_Single_Arm_246 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arm_With_Single_Arm_246"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DuplicateMatchArmVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates a match arm in any match expression with a single arm, creating two identical arms. This tests the compiler's pattern overlap checks and MIR construction by introducing redundant patterns, potentially leading to internal errors or incorrect behavior in pattern matching analysis."
    }
}