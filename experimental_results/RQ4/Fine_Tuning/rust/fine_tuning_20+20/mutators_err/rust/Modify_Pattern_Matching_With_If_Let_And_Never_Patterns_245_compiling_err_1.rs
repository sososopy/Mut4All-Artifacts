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

pub struct Modify_Pattern_Matching_With_If_Let_And_Never_Patterns_245;

impl Mutator for Modify_Pattern_Matching_With_If_Let_And_Never_Patterns_245 {
    fn name(&self) -> &str {
        "Modify_Pattern_Matching_With_If_Let_And_Never_Patterns_245"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchFinder {
            match_spans: Vec<Span>,
        }
        impl<'ast> Visit<'ast> for MatchFinder {
            fn visit_expr_match(&mut self, node: &'ast ExprMatch) {
                self.match_spans.push(node.span());
                syn::visit::visit_expr_match(self, node);
            }
        }
        struct MatchReplacer {
            target_span: Span,
        }
        impl VisitMut for MatchReplacer {
            fn visit_expr_match_mut(&mut self, node: &mut ExprMatch) {
                if node.span() == self.target_span {
                    for arm in &mut node.arms {
                        if let Some((_, ref mut guard)) = arm.guard {
                            if let Expr::Let(expr_let) = &mut **guard {
                                let impossible_pat: Pat = parse_quote!(None);
                                expr_let.pat = impossible_pat;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, node);
            }
        }
        let mut finder = MatchFinder {
            match_spans: Vec::new(),
        };
        finder.visit_file(file);
        let mut rng = thread_rng();
        let target_span = finder.match_spans.choose(&mut rng).cloned();
        if let Some(span) = target_span {
            let mut replacer = MatchReplacer { target_span: span };
            replacer.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets match expressions with `if let` guards, replacing the pattern in the guard with a logically impossible pattern like `None`. This transformation stresses the compiler's pattern matching and guard evaluation logic, potentially revealing issues in pattern exhaustiveness checking and guard execution under edge cases."
    }
}