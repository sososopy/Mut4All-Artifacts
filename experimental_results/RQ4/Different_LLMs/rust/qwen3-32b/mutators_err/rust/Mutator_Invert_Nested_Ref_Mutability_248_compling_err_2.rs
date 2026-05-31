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
                    self.modify_pattern(&mut expr_let.pat);
                } else if let Expr::Match(expr_match) = expr {
                    for arm in &mut expr_match.arms {
                        self.modify_pattern(&mut arm.pat);
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

use syn::PatRef;
use syn::Mutability;

fn collect_ref_pats(pat: &mut Pat) -> Vec<&mut PatRef> {
    let mut result = Vec::new();
    collect_ref_pats_helper(pat, &mut result);
    result
}

fn collect_ref_pats_helper(pat: &mut Pat, result: &mut Vec<&mut PatRef>) {
    match pat {
        Pat::Ref(pat_ref) => {
            result.push(pat_ref);
            collect_ref_pats_helper(&mut pat_ref.pat, result);
        }
        Pat::Wild(_) => {}
        Pat::Ident(pat_ident) => {
            if let Some(subpat) = &mut pat_ident.subpat {
                collect_ref_pats_helper(&mut subpat.1, result);
            }
        }
        Pat::Struct(pat_struct) => {
            for field in &mut pat_struct.fields {
                collect_ref_pats_helper(&mut field.pat, result);
            }
        }
        Pat::TupleStruct(pat_tuple_struct) => {
            for pat in &mut pat_tuple_struct.elems {
                collect_ref_pats_helper(pat, result);
            }
        }
        Pat::Tuple(pat_tuple) => {
            for pat in &mut pat_tuple.elems {
                collect_ref_pats_helper(pat, result);
            }
        }
        Pat::Or(pat_or) => {
            for pat in &mut pat_or.cases {
                collect_ref_pats_helper(pat, result);
            }
        }
        Pat::Lit(_) => {}
        Pat::Range(_) => {}
        Pat::Rest(_) => {}
        Pat::Box(pat_box) => {
            collect_ref_pats_helper(&mut pat_box.pat, result);
        }
        Pat::Slice(pat_slice) => {
            for pat in &mut pat_slice.elems {
                collect_ref_pats_helper(pat, result);
            }
        }
        Pat::Path(_) => {}
        Pat::Lit(_) => {}
        Pat::Macro(_) => {}
        Pat::Verbatim(_) => {}
        Pat::Paren(pat_paren) => {
            collect_ref_pats_helper(&mut pat_paren.pat, result);
        }
        Pat::Never(_) => {}
        Pat::Type(pat_type) => {
            collect_ref_pats_helper(&mut pat_type.pat, result);
        }
        _ => {}
    }
}