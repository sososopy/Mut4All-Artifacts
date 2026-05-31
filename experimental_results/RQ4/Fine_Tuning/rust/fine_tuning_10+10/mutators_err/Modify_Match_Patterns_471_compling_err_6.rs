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

pub struct Modify_Match_Patterns_471;

impl Mutator for Modify_Match_Patterns_471 {
    fn name(&self) -> &str {
        "Modify_Match_Patterns_471"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                        for arm in &mut expr_match.arms {
                            if let Pat::TupleStruct(pat_tuple_struct) = &mut arm.pat {
                                if let Some(first_pat) = pat_tuple_struct.elems.first_mut() {
                                    if let Pat::Range(pat_range) = first_pat {
                                        pat_range.end = Some(Box::new(Pat::Lit(PatLit {
                                            attrs: Vec::new(),
                                            expr: Box::new(Expr::Lit(ExprLit {
                                                lit: syn::Lit::Int(syn::LitInt::new("100", Span::call_site())),
                                            })),
                                        })));
                                    }
                                }
                            }
                            if let Pat::Ident(pat_ident) = &mut arm.pat {
                                if pat_ident.ident == "FOO_REF_REF" {
                                    arm.pat = Pat::Tuple(PatTuple {
                                        attrs: Vec::new(),
                                        elems: Punctuated::from_iter(vec![
                                            Pat::Lit(PatLit {
                                                attrs: Vec::new(),
                                                expr: Box::new(Expr::Lit(ExprLit {
                                                    lit: syn::Lit::Int(syn::LitInt::new("10", Span::call_site())),
                                                })),
                                            }),
                                            Pat::Lit(PatLit {
                                                attrs: Vec::new(),
                                                expr: Box::new(Expr::Lit(ExprLit {
                                                    lit: syn::Lit::Int(syn::LitInt::new("0", Span::call_site())),
                                                })),
                                            }),
                                        ]),
                                        paren_token: token::Paren::default(),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `match` statements, specifically altering patterns that involve ranges and bindings. It replaces range patterns with specific values or overlapping ranges and substitutes bindings with explicit literals. This transformation aims to introduce overlaps or unreachable patterns, potentially triggering compiler warnings or errors related to pattern exhaustiveness and unreachable code."
    }
}