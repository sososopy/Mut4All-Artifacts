use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
use syn::{
    visit::Visit,
    visit_mut::VisitMut,
    *,
    Mutability,
    punctuated::Punctuated,
    spanned::Spanned,
    token::{Paren, Plus},
};

use crate::mutator::Mutator;

pub struct Mutator_Invert_Nested_Ref_Mutability_248;

impl Mutator for Mutator_Invert_Nested_Ref_Mutability_248 {
    fn name(&self) -> &str {
        "Invert_Nested_Ref_Mutability_248"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternMutator;
        impl VisitMut for PatternMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Let(expr_let) = expr {
                    self.visit_pat_mut(&mut expr_let.pat);
                } else if let Expr::Match(expr_match) = expr {
                    for arm in &mut expr_match.arms {
                        self.visit_pat_mut(&mut arm.pat);
                    }
                }
                visit_mut::visit_expr_mut(self, expr);
            }
        }

        impl PatternMutator {
            fn modify_pattern(&mut self, pat: &mut Pat) {
                let mut ref_pats = collect_ref_pats(pat);
                if ref_pats.is_empty() {
                    return;
                }
                let mut rng = thread_rng();
                let target = ref_pats.choose(&mut rng).unwrap();
                target.mutability = match target.mutability {
                    Some(Mutability::Mut) => None,
                    _ => Some(Mutability::Mut),
                };
            }
        }

        let mut visitor = PatternMutator;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inverts the mutability of nested reference patterns in if let and match arms. It randomly selects a level of nested reference and flips its mutability (e.g., &Some(&mut x) becomes &Some(&x)), creating a potential mutability conflict with the corresponding value being matched. This stresses the compiler's pattern matching and mutability consistency checks, potentially exposing bugs in reference handling during pattern binding."
    }
}

fn collect_ref_pats(pat: &mut Pat) -> Vec<&mut PatReference> {
    let mut visitor = CollectRefPatsVisitor { result: Vec::new() };
    visitor.visit_pat_mut(pat);
    visitor.result
}

struct CollectRefPatsVisitor {
    result: Vec<&mut PatReference>,
}

impl VisitMut for CollectRefPatsVisitor {
    fn visit_pat_mut(&mut self, pat: &mut Pat) {
        if let Pat::Reference(pat_ref) = pat {
            self.result.push(pat_ref);
        }
        visit_mut::visit_pat_mut(self, pat);
    }
}