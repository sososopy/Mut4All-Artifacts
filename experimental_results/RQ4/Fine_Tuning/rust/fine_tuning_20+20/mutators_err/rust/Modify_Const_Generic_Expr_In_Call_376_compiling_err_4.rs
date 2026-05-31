use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus, Semi},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Expr_In_Call_376;

impl Mutator for Modify_Const_Generic_Expr_In_Call_376 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Expr_In_Call_376"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LifetimeCollector {
            lifetimes: HashSet<String>,
        }
        impl<'ast> Visit<'ast> for LifetimeCollector {
            fn visit_lifetime(&mut self, lifetime: &'ast Lifetime) {
                self.lifetimes.insert(lifetime.ident.to_string());
            }
        }
        struct ConstExprTransformer {
            lifetimes: HashSet<String>,
        }
        impl VisitMut for ConstExprTransformer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Block(expr_block) = expr {
                    let mut new_block = expr_block.clone();
                    new_block.block.stmts.clear();
                    let lifetime_ident = self
                        .lifetimes
                        .iter()
                        .next()
                        .cloned()
                        .unwrap_or_else(|| "'static".to_string());
                    let lifetime_stmt: Stmt = parse_quote! { let _: & #lifetime_ident (); };
                    new_block.block.stmts.push(Stmt::Local(lifetime_stmt));
                    let tmp_ident = Ident::new("tmp", Span::call_site());
                    let tmp_pat: Pat = parse_quote! { #tmp_ident };
                    let tmp_local = Local {
                        attrs: Vec::new(),
                        let_token: Default::default(),
                        pat: tmp_pat,
                        init: expr_block.block.stmts.first().and_then(|stmt| match stmt {
                            Stmt::Expr(expr, _) => Some(LocalInit {
                                eq_token: Default::default(),
                                expr: Box::new(expr.clone()),
                                diverge: None,
                            }),
                            Stmt::Semi(expr, _) => Some(LocalInit {
                                eq_token: Default::default(),
                                expr: Box::new(expr.clone()),
                                diverge: None,
                            }),
                            _ => None,
                        }),
                        semi_token: Default::default(),
                    };
                    new_block.block.stmts.push(Stmt::Local(tmp_local));
                    let tmp_expr: Expr = parse_quote! { #tmp_ident };
                    new_block.block.stmts.push(Stmt::Expr(tmp_expr, None));
                    *expr = Expr::Block(new_block);
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        struct ConstGenericCallTransformer {
            lifetimes: HashSet<String>,
        }
        impl VisitMut for ConstGenericCallTransformer {
            fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
                let mut transformed = false;
                for arg in &mut expr_call.args {
                    if let Expr::Path(expr_path) = arg {
                        if expr_path.qself.is_none()
                            && expr_path.path.segments.len() == 1
                            && expr_path.path.segments[0].ident == "N"
                        {
                            let new_expr: Expr = parse_quote! { { 3 } };
                            *arg = new_expr;
                            transformed = true;
                        }
                    }
                }
                if transformed {
                    let mut transformer = ConstExprTransformer {
                        lifetimes: self.lifetimes.clone(),
                    };
                    transformer.visit_expr_call_mut(expr_call);
                }
                syn::visit_mut::visit_expr_call_mut(self, expr_call);
            }
        }
        let mut collector = LifetimeCollector {
            lifetimes: HashSet::new(),
        };
        collector.visit_file(file);
        let mut transformer = ConstGenericCallTransformer {
            lifetimes: collector.lifetimes,
        };
        transformer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function calls with constant generic parameters by transforming the constant expression into a block that introduces a temporary variable and references an existing lifetime parameter. It aims to stress-test the compiler's handling of constant generics, lifetimes, and temporary variables within constant expressions."
    }
}