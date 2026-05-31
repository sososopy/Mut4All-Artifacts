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

pub struct Duplicate_Match_Arms_367;

impl Mutator for Duplicate_Match_Arms_367 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arms_367"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchArmDuplicator;
        impl VisitMut for MatchArmDuplicator {
            fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
                if let Some(arm) = expr_match.arms.first().cloned() {
                    expr_match.arms.insert(0, arm);
                }
                syn::visit_mut::visit_expr_match_mut(self, expr_match);
            }
        }
        MatchArmDuplicator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions in Rust functions. It duplicates an existing match arm, including its pattern and guard, within the same match expression. This aims to explore potential logical duplications and evaluate how the compiler handles such scenarios."
    }
}