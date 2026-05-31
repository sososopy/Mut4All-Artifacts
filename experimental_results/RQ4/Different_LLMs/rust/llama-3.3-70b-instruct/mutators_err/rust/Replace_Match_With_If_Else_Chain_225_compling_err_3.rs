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

pub struct Replace_Match_With_If_Else_Chain_225;

impl Mutator for Replace_Match_With_If_Else_Chain_225 {
    fn name(&self) -> &str {
        "Replace_Match_With_If_Else_Chain_225"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Match(expr_match) = expr {
                            let mut if_else_chain = Vec::new();
                            for arm in &expr_match.arms {
                                let pattern = &arm.pat;
                                let guard = &arm.guard;
                                let body = &arm.body;
                                let if_stmt = if let Some(guard) = guard {
                                    quote! {
                                        if #pattern == #expr_match.expr {
                                            #body
                                        } else if #guard {
                                            #body
                                        }
                                    }
                                } else {
                                    quote! {
                                        if #pattern == #expr_match.expr {
                                            #body
                                        }
                                    }
                                };
                                if_else_chain.push(if_stmt);
                            }
                            let if_else_chain = if_else_chain.into_iter().reduce(|acc, elem| {
                                quote! {
                                    #acc
                                    #elem
                                }
                            });
                            if let Some(if_else_chain) = if_else_chain {
                                *stmt = Stmt::Expr(if_else_chain, expr.span());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces match statements with if-else chains. This transformation tests the compiler's handling of control flow and conditional statements, potentially revealing issues in optimization or validation phases."
    }
}