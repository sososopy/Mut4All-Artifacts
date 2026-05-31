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

pub struct Conditional_Statement_Negation_220;

impl Mutator for Conditional_Statement_Negation_220 {
    fn name(&self) -> &str {
        "Conditional_Statement_Negation_220"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::If(if_expr) = &**expr {
                            let negated_cond = negate_condition(if_expr.cond.clone());
                            stmt.expr = parse_quote! {
                                if #negated_cond {
                                    #if_expr.then_branch
                                } else {
                                    #if_expr.else_branch
                                }
                            };
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets conditional statements in the code and negates their conditions. It applies to if statements and match statements with guard clauses. The goal is to test how the compiler handles different branching logic, especially in optimized MIR."
    }
}

fn negate_condition(cond: syn::Expr) -> syn::Expr {
    match cond {
        Expr::Unary(unary) => {
            if let token::Not(_) = unary.op {
                unary.expr.clone()
            } else {
                parse_quote! { ! #cond }
            }
        }
        Expr::Binary(binary) => {
            match binary.op {
                token::Eq(_) => parse_quote! { #binary.left != #binary.right },
                token::NotEq(_) => parse_quote! { #binary.left == #binary.right },
                token::Lt(_) => parse_quote! { #binary.left >= #binary.right },
                token::Le(_) => parse_quote! { #binary.left > #binary.right },
                token::Gt(_) => parse_quote! { #binary.left <= #binary.right },
                token::Ge(_) => parse_quote! { #binary.left < #binary.right },
                _ => cond,
            }
        }
        _ => parse_quote! { ! #cond },
    }
}