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

pub struct Trivial_Bounds_Const_Eval_190;

impl Mutator for Trivial_Bounds_Const_Eval_190 {
    fn name(&self) -> &str {
        "Trivial_Bounds_Const_Eval_190"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found_sized = false;
                let mut found_copy = false;
                if let Some(where_clause) = &item_fn.sig.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound
                                        .path
                                        .segments
                                        .last()
                                        .map_or(false, |segment| {
                                            segment.ident == "Sized"
                                        })
                                    {
                                        found_sized = true;
                                    }
                                    if trait_bound
                                        .path
                                        .segments
                                        .last()
                                        .map_or(false, |segment| {
                                            segment.ident == "Copy"
                                        })
                                    {
                                        found_copy = true;
                                    }
                                }
                            }
                        }
                    }
                }
                if found_sized {
                    let trivial_expr: Expr = parse_quote! {
                        [(); { let _temp: Option<str> = None; 0 }]
                    };
                    item_fn.block.stmts.push(Stmt::Expr(trivial_expr, None));
                } else if found_copy {
                    let trivial_expr: Expr = parse_quote! {
                        [(); { let _temp: Option<str> = None; 0 }]
                    };
                    item_fn.block.stmts.push(Stmt::Expr(trivial_expr, None));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with type constraints involving `Sized` or `Copy`. It injects a const-evaluated array expression that relies on a trivial bound for a non-sized type like `str`. By forcing the compiler to evaluate a const expression with a type that violates typical constraints, it aims to uncover bugs in const evaluation and type checking, particularly those related to the `trivial_bounds` feature."
    }
}