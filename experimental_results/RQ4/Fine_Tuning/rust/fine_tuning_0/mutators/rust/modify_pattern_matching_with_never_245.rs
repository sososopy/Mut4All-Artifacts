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

pub struct Modify_Pattern_Matching_With_Never_245;

impl Mutator for Modify_Pattern_Matching_With_Never_245 {
    fn name(&self) -> &str {
        "Modify_Pattern_Matching_With_Never_245"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                        for arm in &mut expr_match.arms {
                            if let Some((_, guard)) = &arm.guard {
                                if let Expr::Let(expr_let) = &**guard {
                                    if let Pat::TupleStruct(pat_tuple_struct) = &*expr_let.pat {
                                        if pat_tuple_struct.path.is_ident("Some") {
                                            let impossible_pat: Pat = parse_quote!(None);
                                            arm.guard = Some((token::If::default(), Box::new(Expr::Let(ExprLet {
                                                attrs: Vec::new(),
                                                let_token: token::Let::default(),
                                                pat: Box::new(impossible_pat),
                                                eq_token: token::Eq::default(),
                                                expr: expr_let.expr.clone(),
                                            }))));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions with `if let` guards. It replaces match arm patterns in `if let` guards with logically impossible patterns, such as `None` for a `Some` pattern. This transformation challenges the compiler's pattern matching logic, potentially uncovering bugs in guard evaluation and pattern checking."
    }
}