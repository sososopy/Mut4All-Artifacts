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

pub struct Swap_Tuple_Matching_With_Destructuring_26;

impl Mutator for Swap_Tuple_Matching_With_Destructuring_26 {
    fn name(&self) -> &str {
        "Swap_Tuple_Matching_With_Destructuring_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = TuplePatternSwapper;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of tuple elements in pattern matching scenarios within function bodies. It targets let bindings and destructuring assignments, aiming to expose potential issues with type inference and pattern matching in Rust's type system."
    }
}

struct TuplePatternSwapper;

impl VisitMut for TuplePatternSwapper {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Pat::Tuple(tuple_pat) = &mut local.pat {
            if let Some(init) = &mut local.init {
                if let Expr::Tuple(tuple_expr) = init.expr.as_mut() {
                    if tuple_pat.elems.len() == tuple_expr.elems.len() && tuple_pat.elems.len() > 1 {
                        let elems_pat: Vec<_> = tuple_pat.elems.iter().cloned().collect();
                        let elems_expr: Vec<_> = tuple_expr.elems.iter().cloned().collect();
                        tuple_pat.elems.clear();
                        tuple_expr.elems.clear();
                        tuple_pat.elems.extend(vec![elems_pat[1].clone(), elems_pat[0].clone()]);
                        tuple_expr.elems.extend(vec![elems_expr[1].clone(), elems_expr[0].clone()]);
                    }
                }
            }
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
}