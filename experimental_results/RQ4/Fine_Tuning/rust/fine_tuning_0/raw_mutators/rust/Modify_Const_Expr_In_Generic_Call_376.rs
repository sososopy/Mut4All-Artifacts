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

pub struct Modify_Const_Expr_In_Generic_Call_376;

impl Mutator for Modify_Const_Expr_In_Generic_Call_376 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Generic_Call_376"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let lifetimes: Vec<&Lifetime> = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            Some(&lifetime_param.lifetime)
                        } else {
                            None
                        }
                    })
                    .collect();

                let mut visitor = ConstExprVisitor {
                    lifetimes: &lifetimes,
                };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ConstExprVisitor<'a> {
    lifetimes: &'a [&'a Lifetime],
}

impl<'a> VisitMut for ConstExprVisitor<'a> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Some(GenericArgument::Const(expr)) = node.args.iter_mut().find_map(|arg| {
            if let Expr::Block(expr_block) = arg {
                Some(GenericArgument::Const(Expr::Block(expr_block.clone())))
            } else {
                None
            }
        }) {
            if let Expr::Block(expr_block) = expr {
                let span = expr_block.span();
                let lifetime_expr: Expr = if let Some(lifetime) = self.lifetimes.first() {
                    parse_quote_spanned! {span=> let _: &'#lifetime (); }
                } else {
                    parse_quote_spanned! {span=> let _: &'static (); }
                };
                let tmp_expr: Expr = parse_quote_spanned! {span=> let tmp = 3; tmp };
                expr_block.block.stmts.insert(0, Stmt::Expr(lifetime_expr));
                expr_block.block.stmts.push(Stmt::Expr(tmp_expr));
            }
        }
        visit_mut::visit_expr_call_mut(self, node);
    }
}