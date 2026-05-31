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

pub struct Replace_Recursive_Call_With_Loop_34;

impl Mutator for Replace_Recursive_Call_With_Loop_34 {
    fn name(&self) -> &str {
        "Replace_Recursive_Call_With_Loop_34"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut new_block = func.block.clone();
                new_block.stmts.clear();
                let mut loop_cond = Expr::Verbatim(quote!(true));
                let mut loop_body_stmts = Punctuated::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call_expr) = expr {
                            if let Expr::Path(path_expr) = &*call_expr.func {
                                if path_expr.path.segments.last().unwrap().ident == func.sig.ident {
                                    loop_cond = Expr::Verbatim(quote!(true));
                                    loop_body_stmts.push(Stmt::Expr(expr.clone(), None));
                                }
                            }
                        }
                    }
                }
                let loop_body = Block {
                    brace_token: token::Brace {
                        span: Span::call_site(),
                    },
                    stmts: loop_body_stmts.into_iter().collect(),
                };
                new_block.stmts.push(Stmt::Expr(Expr::Verbatim(quote!({
                    loop {
                        if !#loop_cond {
                            break;
                        }
                        #loop_body
                    }
                })), None));
                func.block = new_block;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces recursive function calls with loops, altering the control flow and potentially exposing borrow checker errors. It targets recursive functions called within loops or conditional statements, replacing the recursive call with a loop that continues until a certain condition is met."
    }
}