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

pub struct Replace_Generic_Constraint_363;

impl Mutator for Replace_Generic_Constraint_363 {
    fn name(&self) -> &str {
        "Replace_Generic_Constraint_363"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &func.sig.generics {
                    let generics = generics.clone();
                    for param in &generics.params {
                        if let GenericParam::Type(param) = param {
                            if let Some(where_clause) = &generics.where_clause {
                                let where_clause = where_clause.clone();
                                for predicate in &where_clause.predicates {
                                    if let WherePredicate::Type(predicate) = predicate {
                                        if let Type::Path(type_path) = &predicate.bounded_ty {
                                            if let Some(const_expr) = find_const_expr_in_type_path(&type_path) {
                                                replace_const_expr(const_expr, predicate);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &impl_item.generics {
                    let generics = generics.clone();
                    for param in &generics.params {
                        if let GenericParam::Type(param) = param {
                            if let Some(where_clause) = &generics.where_clause {
                                let where_clause = where_clause.clone();
                                for predicate in &where_clause.predicates {
                                    if let WherePredicate::Type(predicate) = predicate {
                                        if let Type::Path(type_path) = &predicate.bounded_ty {
                                            if let Some(const_expr) = find_const_expr_in_type_path(&type_path) {
                                                replace_const_expr(const_expr, predicate);
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
        "The mutation operator targets generic constraints in Rust code, specifically focusing on const generic expressions. It replaces these constraints with alternative ones, potentially altering the const expression, the type of the variable, or the lifetime parameters. This transformation aims to test the compiler's handling of different generic const expression scenarios and potentially trigger similar bugs or errors."
    }
}

fn find_const_expr_in_type_path(type_path: &TypePath) -> Option<&Expr> {
    for segment in &type_path.path.segments {
        if let PathArguments::AngleBracketed(args) = &segment.arguments {
            for arg in args.args.iter() {
                if let GenericArgument::Type(Type::Array(array)) = arg {
                    if let Some(const_expr) = find_const_expr_in_array(array) {
                        return Some(const_expr);
                    }
                }
            }
        }
    }
    None
}

fn find_const_expr_in_array(array: &syn::TypeArray) -> Option<&Expr> {
    if let Some(len) = &array.len {
        if let Expr::Block(block) = len {
            if let Some(const_expr) = block.block.stmts.last() {
                if let Stmt::Expr(expr, _) = const_expr {
                    return Some(&expr);
                }
            }
        }
    }
    None
}

fn replace_const_expr(const_expr: &Expr, predicate: &WherePredicate) {
    let new_const_expr = create_new_const_expr(const_expr);
    if let Type::Path(type_path) = &predicate.bounded_ty {
        for segment in &type_path.path.segments {
            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                for arg in args.args.iter() {
                    if let GenericArgument::Type(Type::Array(array)) = arg {
                        if let Some(len) = &array.len {
                            if let Expr::Block(block) = len {
                                if let Some(last_stmt) = block.block.stmts.last() {
                                    if let Stmt::Expr(last_expr, _) = last_stmt {
                                        let mut new_last_expr = last_expr.clone();
                                        new_last_expr.clone_from(&new_const_expr);
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

fn create_new_const_expr(const_expr: &Expr) -> Expr {
    let mut rng = thread_rng();
    let new_const_expr = match rng.gen_range(0..3) {
        0 => {
            let new_value = rng.gen_range(0..10);
            parse_quote! { #new_value }
        }
        1 => {
            let new_value = rng.gen_range(0..10);
            parse_quote! { { #new_value } }
        }
        2 => {
            let new_value = rng.gen_range(0..10);
            parse_quote! { { |x: &'static u32| { x }; #new_value } }
        }
        _ => unreachable!(),
    };
    new_const_expr
}