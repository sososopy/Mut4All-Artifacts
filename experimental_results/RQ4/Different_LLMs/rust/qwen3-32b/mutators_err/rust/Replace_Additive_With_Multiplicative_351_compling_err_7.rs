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

pub struct Replace_Additive_With_Multiplicative_351;

impl Mutator for Replace_Additive_With_Multiplicative_351 {
    fn name(&self) -> &str {
        "Replace_Additive_With_Multiplicative_351"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Array(array_type) = Box::new(predicate_type.bounded_ty).as_mut() {
                                if let syn::GenericArgument::Const(expr_box) = &mut array_type.len {
                                    let expr = Box::as_mut(expr_box);
                                    if let syn::Expr::Binary(expr_binary) = expr {
                                        match expr_binary.op {
                                            syn::BinOp::Add(_) => {
                                                expr_binary.op = syn::BinOp::Mul(syn::token::Star::default());
                                            },
                                            syn::BinOp::Sub(_) => {
                                                expr_binary.op = syn::BinOp::Mul(syn::token::Star::default());
                                            },
                                            _ => {}
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
        ""
    }
}