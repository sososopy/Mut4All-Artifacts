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

pub struct Modify_Pattern_Matching_Operator_50;

impl Mutator for Modify_Pattern_Matching_Operator_50 {
    fn name(&self) -> &str {
        "Modify_Pattern_Matching_Operator_50"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Match(expr_match) = expr {
                            let mut new_arms = expr_match.arms.clone();
                            let mut rng = thread_rng();
                            let new_pattern = match rng.gen_range(0..2) {
                                0 => {
                                    // Replace the existing pattern with a new constant pattern
                                    Pat::Lit(Lit::Int(LitInt::new(
                                        &rng.gen_range(0..100).to_string(),
                                        Span::call_site(),
                                    )))
                                }
                                1 => {
                                    // Replace the existing pattern with a new variable pattern
                                    Pat::Ident(Ident::new(
                                        &format!("var_{}", rng.gen_range(0..100)),
                                        Span::call_site(),
                                    ))
                                }
                                _ => unreachable!(),
                            };
                            new_arms.push(Arm {
                                attrs: vec![],
                                pat: new_pattern,
                                guard: None,
                                comma: Some(Comma::default()),
                                fat_arrow_token: token::FatArrow {
                                    spans: [Span::call_site(), Span::call_site()],
                                },
                                body: Box::new(Expr::Block(ExprBlock {
                                    brace_token: token::Brace {
                                        span: Span::call_site(),
                                    },
                                    stmts: vec![Stmt::Expr(
                                        parse_quote!(println!("new arm")),
                                        None,
                                    )],
                                })),
                            });
                            *expr = Expr::Match(ExprMatch {
                                attrs: expr_match.attrs,
                                match_token: expr_match.match_token,
                                expr: expr_match.expr,
                                brace_token: token::Brace {
                                    span: Span::call_site(),
                                },
                                arms: new_arms,
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions and modifies their patterns. It either replaces the existing pattern with a new constant pattern or adds a new arm with a constant or variable pattern. This transformation aims to increase the diversity of patterns used in match expressions, which could help expose bugs related to constant evaluation and pattern matching."
    }
}