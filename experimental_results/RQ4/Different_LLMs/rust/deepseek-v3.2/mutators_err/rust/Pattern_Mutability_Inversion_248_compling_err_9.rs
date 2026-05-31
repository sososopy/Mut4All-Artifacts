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

pub struct Pattern_Mutability_Inversion_248;

impl Mutator for Pattern_Mutability_Inversion_248 {
    fn name(&self) -> &str {
        "Pattern_Mutability_Inversion_248"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternMutabilityVisitor;
        impl VisitMut for PatternMutabilityVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                match expr {
                    Expr::Match(expr_match) => {
                        for arm in &mut expr_match.arms {
                            self.visit_pat_mut(&mut arm.pat);
                        }
                        self.visit_expr_mut(&mut expr_match.expr);
                    }
                    Expr::If(expr_if) => {
                        if let Some(expr_if_let) = match &mut *expr_if.cond {
                            Expr::Let(expr_let) => Some(expr_let),
                            _ => None,
                        } {
                            self.visit_pat_mut(&mut expr_if_let.pat);
                            self.visit_expr_mut(&mut expr_if_let.expr);
                        }
                        self.visit_block_mut(&mut expr_if.then_branch);
                        if let Some((_, else_branch)) = &mut expr_if.else_branch {
                            self.visit_expr_mut(else_branch);
                        }
                    }
                    Expr::Let(expr_let) => {
                        self.visit_pat_mut(&mut expr_let.pat);
                        self.visit_expr_mut(&mut expr_let.expr);
                    }
                    _ => {}
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
            fn visit_pat_mut(&mut self, pat: &mut Pat) {
                match pat {
                    Pat::Reference(pat_ref) => {
                        if pat_ref.mutability.is_some() {
                            pat_ref.mutability = None;
                        }
                        self.visit_pat_mut(&mut pat_ref.pat);
                    }
                    Pat::Struct(pat_struct) => {
                        for field in &mut pat_struct.fields {
                            self.visit_pat_mut(&mut field.pat);
                        }
                    }
                    Pat::TupleStruct(pat_tuple_struct) => {
                        for elem in &mut pat_tuple_struct.elems {
                            self.visit_pat_mut(elem);
                        }
                    }
                    Pat::Tuple(pat_tuple) => {
                        for elem in &mut pat_tuple.elems {
                            self.visit_pat_mut(elem);
                        }
                    }
                    Pat::Slice(pat_slice) => {
                        for elem in &mut pat_slice.elems {
                            self.visit_pat_mut(elem);
                        }
                    }
                    Pat::Or(pat_or) => {
                        for alt in &mut pat_or.cases {
                            self.visit_pat_mut(alt);
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_pat_mut(self, pat);
            }
        }
        let mut visitor = PatternMutabilityVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets pattern matching expressions involving mutable references. It traverses patterns in let statements, match arms, and if let expressions, and when it encounters a mutable reference pattern (`&mut`), it removes the mutability qualifier, turning it into a shared reference pattern (`&`). This inversion violates compiler assumptions about mutability caps in nested pattern analysis, potentially triggering internal compiler errors related to pattern mutability violations. The change is made directly within the pattern syntax without altering the overall structure or introducing new elements."
    }
}