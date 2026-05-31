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

pub struct Expand_Or_Pattern_688;

impl Mutator for Expand_Or_Pattern_688 {
    fn name(&self) -> &str {
        "Expand_Or_Pattern_688"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = OrPatternExpander;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator expands or-patterns in match arms into separate arms. This transformation tests the compiler's handling of pattern matching logic by ensuring that each case is explicitly handled, potentially uncovering issues with pattern matching optimization or logic."
    }
}

struct OrPatternExpander;

impl VisitMut for OrPatternExpander {
    fn visit_expr_match_mut(&mut self, i: &mut syn::ExprMatch) {
        let mut new_arms = Vec::new();
        for arm in &i.arms {
            if let Pat::Or(pat_or) = &arm.pat {
                for pat in &pat_or.cases {
                    let new_arm = syn::Arm {
                        pat: pat.clone(),
                        guard: arm.guard.clone(),
                        body: arm.body.clone(),
                        fat_arrow_token: arm.fat_arrow_token,
                        attrs: arm.attrs.clone(),
                        comma: arm.comma,
                    };
                    new_arms.push(new_arm);
                }
            } else {
                new_arms.push(arm.clone());
            }
        }
        i.arms = new_arms;
    }
}