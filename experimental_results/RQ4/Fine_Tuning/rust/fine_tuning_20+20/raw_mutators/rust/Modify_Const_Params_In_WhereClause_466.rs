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

pub struct Modify_Const_Params_In_WhereClause_466;

impl Mutator for Modify_Const_Params_In_WhereClause_466 {
    fn name(&self) -> &str {
        "Modify_Const_Params_In_WhereClause_466"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    for segment in &mut trait_bound.path.segments {
                                        if let PathArguments::AngleBracketed(
                                            angle_bracketed_args,
                                        ) = &mut segment.arguments
                                        {
                                            for arg in &mut angle_bracketed_args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Block(expr_block) = expr {
                                                        if expr_block.block.stmts.len() == 1 {
                                                            if let Stmt::Expr(expr, _) =
                                                                &expr_block.block.stmts[0]
                                                            {
                                                                if let Expr::Lit(expr_lit) = expr {
                                                                    if let syn::Lit::Int(lit_int) =
                                                                        &expr_lit.lit
                                                                    {
                                                                        let new_expr = if lit_int
                                                                            .suffix()
                                                                            .starts_with('u')
                                                                        {
                                                                            let new_lit = syn::LitInt::new(
                                                                                &format!(
                                                                                    "{}u32",
                                                                                    lit_int
                                                                                        .base10_digits()
                                                                                ),
                                                                                lit_int.span(),
                                                                            );
                                                                            Expr::Lit(
                                                                                syn::ExprLit {
                                                                                    attrs: Vec::new(),
                                                                                    lit: syn::Lit::Int(
                                                                                        new_lit,
                                                                                    ),
                                                                                },
                                                                            )
                                                                        } else {
                                                                            Expr::Lit(
                                                                                syn::ExprLit {
                                                                                    attrs: Vec::new(),
                                                                                    lit: syn::Lit::Bool(
                                                                                        syn::LitBool {
                                                                                            value: false,
                                                                                            span: lit_int
                                                                                                .span(),
                                                                                        },
                                                                                    ),
                                                                                },
                                                                            )
                                                                        };
                                                                        expr_block.block.stmts[0] =
                                                                            Stmt::Expr(
                                                                                new_expr, None,
                                                                            );
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
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic const parameters in their `where` clauses, specifically those using const expressions in trait bounds. It replaces integer const expressions with either a different integer type (e.g., `u32`) or a boolean expression (e.g., `false`). This transformation aims to exploit potential weaknesses in the compiler's handling of const generics and type constraints, particularly in how it resolves and checks const expressions within type bounds."
    }
}