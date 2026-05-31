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

pub struct Swap_If_Else_Blocks_115;

impl Mutator for Swap_If_Else_Blocks_115 {
    fn name(&self) -> &str {
        "Swap_If_Else_Blocks_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Expr(Expr::If(expr_if)) = stmt {
                        if let Some((_, else_block)) = &expr_if.else_branch {
                            let swapped_if = ExprIf {
                                cond: expr_if.cond.clone(),
                                then_branch: match **else_block {
                                    Expr::Block(ref block) => block.clone(),
                                    _ => continue,
                                },
                                else_branch: Some((token::Else { span: Span::call_site() }, Box::new(Expr::Block(expr_if.then_branch.clone())))),
                                ..expr_if.clone()
                            };
                            new_stmts.push(Stmt::Expr(Expr::If(swapped_if)));
                        } else {
                            new_stmts.push(stmt.clone());
                        }
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator swaps the blocks of `if` and `else` branches in function bodies. By altering the control flow logic, it challenges the compiler's optimization and control flow analysis, potentially revealing hidden assumptions or bugs in handling conditional expressions."
    }
}