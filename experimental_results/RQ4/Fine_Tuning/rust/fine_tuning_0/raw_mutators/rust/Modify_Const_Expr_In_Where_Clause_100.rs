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

pub struct Modify_Const_Expr_In_Where_Clause_100;

impl Mutator for Modify_Const_Expr_In_Where_Clause_100 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Where_Clause_100"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Lit(expr_lit) = expr {
                                                        if let syn::Lit::Bool(_) = &expr_lit.lit {
                                                            *expr = parse_quote! { ASSUME_ALIGNMENT && some_other_condition() };
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions in the where clause of functions using const generics. It replaces simple boolean constants with complex expressions that depend on runtime conditions, enforcing non-constant evaluation and potentially leading to ICEs in scenarios where compile-time evaluation is expected."
    }
}

fn some_other_condition() -> bool {
    use std::time::{SystemTime, UNIX_EPOCH};
    let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
    now.as_secs() % 2 == 0
}