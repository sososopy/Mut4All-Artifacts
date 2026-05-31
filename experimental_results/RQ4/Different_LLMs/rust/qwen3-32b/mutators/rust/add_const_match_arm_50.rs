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

pub struct Add_Const_Match_Arm_50;

impl Mutator for Add_Const_Match_Arm_50 {
    fn name(&self) -> &str {
        "Add_Const_Match_Arm_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect const and static identifiers in the file
        let mut const_idents = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                const_idents.insert(item_const.ident.clone());
            } else if let Item::Static(item_static) = item {
                const_idents.insert(item_static.ident.clone());
            }
        }

        // Traverse the file to find match expressions and add arms
        struct MatchFinder<'a> {
            const_idents: &'a HashSet<Ident>,
        }

        impl<'a> VisitMut for MatchFinder<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Match(expr_match) = expr {
                    self.visit_expr_match_mut(expr_match);
                } else {
                    syn::visit_mut::visit_expr_mut(self, expr);
                }
            }

            fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
                // Check if the match expression's value is a constant
                if let Expr::Path(expr_path) = &*expr_match.expr {
                    let path = &expr_path.path;
                    if let Some(segment) = path.segments.last() {
                        if self.const_idents.contains(&segment.ident) {
                            // Add a new match arm with the constant's identifier as the pattern
                            let ident = segment.ident.clone();
                            let new_arm = parse_quote! {
                                #ident => {},
                            };
                            expr_match.arms.push(new_arm);
                        }
                    }
                }
                syn::visit_mut::visit_expr_match_mut(self, expr_match);
            }
        }

        let mut finder = MatchFinder { const_idents: &const_idents };
        finder.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a redundant match arm using the constant's identifier as a pattern in any match expression that evaluates the constant's value. This forces the compiler to perform const evaluation during pattern matching, potentially leading to infinite recursion or ICEs when the constant's value is complex or involves scalar pairs."
    }
}