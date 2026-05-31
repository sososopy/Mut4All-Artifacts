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

pub struct Replace_Match_Arm_With_Catch_All_202;

impl Mutator for Replace_Match_Arm_With_Catch_All_202 {
    fn name(&self) -> &str {
        "Replace_Match_Arm_With_Catch_All_202"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Match(expr_match) = &**expr {
                            if let Some((index, _)) = expr_match.arms.iter().enumerate().choose(&mut thread_rng()) {
                                expr_match.arms[index] = parse_quote! { _ => () };
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a randomly chosen match arm with a catch-all arm (_). This transformation tests the compiler's handling of pattern matching and may lead to unexpected behavior or errors if the replaced arm was essential to the program's logic."
    }
}