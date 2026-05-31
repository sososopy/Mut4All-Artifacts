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

pub struct Perturb_Loop_Control_Flow_601;

impl Mutator for Perturb_Loop_Control_Flow_601 {
    fn name(&self) -> &str {
        "Perturb_Loop_Control_Flow_601"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::While(expr_while)) = stmt {
                        let condition = expr_while.cond.clone();
                        let new_stmt: Stmt = parse_quote! {
                            if #condition {
                                break;
                            }
                        };
                        expr_while.body.stmts.insert(0, new_stmt);
                    } else if let Stmt::Expr(Expr::ForLoop(expr_for)) = stmt {
                        let new_stmt: Stmt = parse_quote! {
                            if true {
                                break;
                            }
                        };
                        expr_for.body.stmts.insert(0, new_stmt);
                    } else if let Stmt::Expr(Expr::Loop(expr_loop)) = stmt {
                        let new_stmt: Stmt = parse_quote! {
                            if false {
                                break;
                            }
                        };
                        expr_loop.body.stmts.insert(0, new_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}