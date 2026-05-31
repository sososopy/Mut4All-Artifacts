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

pub struct Duplicate_Match_Arms_246;

impl Mutator for Duplicate_Match_Arms_246 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arms_246"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_arms = Vec::new();
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                        for arm in &expr_match.arms {
                            let new_arm = syn::Arm {
                                pat: arm.pat.clone(),
                                guard: arm.guard.clone(),
                                body: arm.body.clone(),
                                ..arm.clone()
                            };
                            new_arms.push(new_arm);
                        }
                        expr_match.arms.extend(new_arms.clone());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator duplicates match arms within a match expression, replicating both the pattern and guard condition. It aims to test the compiler's handling of duplicated logical paths and match evaluation, potentially leading to unexpected behavior or crashes due to logical redundancy."
    }
}