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

pub struct Duplicate_Match_Arms_With_Identical_Conditions_246;

impl Mutator for Duplicate_Match_Arms_With_Identical_Conditions_246 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arms_With_Identical_Conditions_246"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut target_index = None;
                let mut has_match = false;
                for (stmt_index, stmt) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Expr(Expr::Match(_), _) = stmt {
                        has_match = true;
                        target_index = Some(stmt_index);
                        break;
                    }
                }
                if !has_match {
                    continue;
                }
                if let Some(index) = target_index {
                    if let Stmt::Expr(Expr::Match(expr_match), _) =
                        &mut item_fn.block.stmts[index]
                    {
                        let mut rng = thread_rng();
                        let arm_count = expr_match.arms.len();
                        if arm_count == 0 {
                            return;
                        }
                        let target_arm_index = rng.gen_range(0..arm_count);
                        let target_arm = expr_match.arms[target_arm_index].clone();
                        expr_match.arms.insert(target_arm_index, target_arm);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions containing match expressions. It duplicates a randomly selected match arm, including its pattern, guard, and body, within the match block. By introducing identical match arms, it stresses the compiler's pattern matching logic, potentially exposing issues in pattern resolution, guard evaluation, and code generation for match expressions."
    }
}