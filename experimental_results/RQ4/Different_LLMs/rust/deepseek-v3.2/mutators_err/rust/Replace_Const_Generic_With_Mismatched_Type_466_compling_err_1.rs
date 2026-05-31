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

pub struct Replace_Const_Generic_With_Mismatched_Type_466;

impl Mutator for Replace_Const_Generic_With_Mismatched_Type_466 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Mismatched_Type_466"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceConstGenericVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls with const generic arguments used in transmutability trait bounds. It identifies boolean const generic arguments and replaces them with integer const expressions of mismatched types (e.g., u8, i32, usize). This creates type mismatches in const generic parameter expectations, testing the compiler's const generic type checking, trait bound resolution, and transmutability analysis under invalid type conditions."
    }
}

struct ReplaceConstGenericVisitor;

impl ReplaceConstGenericVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for ReplaceConstGenericVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if let Some(last_segment) = path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                    let mut rng = thread_rng();
                    let mut candidates = Vec::new();
                    
                    for (i, arg) in args.args.iter().enumerate() {
                        if let GenericArgument::Const(const_expr) = arg {
                            if self.is_bool_const(const_expr) {
                                candidates.push(i);
                            }
                        }
                    }
                    
                    if !candidates.is_empty() {
                        let chosen_idx = *candidates.choose(&mut rng).unwrap();
                        let new_expr = self.generate_mismatched_const_expr(&mut rng);
                        
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            if let Some(GenericArgument::Const(old_expr)) = args.args.get_mut(chosen_idx) {
                                *old_expr = new_expr;
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, node);
    }
}

impl ReplaceConstGenericVisitor {
    fn is_bool_const(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Lit(lit) => {
                if let Lit::Bool(_) = lit.lit {
                    return true;
                }
                false
            }
            Expr::Path(path) => {
                if let Some(segment) = path.path.segments.last() {
                    let ident_str = segment.ident.to_string();
                    ident_str == "true" || ident_str == "false"
                } else {
                    false
                }
            }
            _ => false,
        }
    }
    
    fn generate_mismatched_const_expr(&self, rng: &mut impl Rng) -> Expr {
        let choices = [
            parse_quote!({ 0u8 }),
            parse_quote!({ 1u8 }),
            parse_quote!({ 0i32 }),
            parse_quote!({ 1i32 }),
            parse_quote!({ 0usize }),
            parse_quote!({ 1usize }),
            parse_quote!({ 42u16 }),
            parse_quote!({ 99i64 }),
        ];
        choices.choose(rng).unwrap().clone()
    }
}