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

fn is_unsafe_expr(expr: &Expr) -> bool {
    matches!(expr, Expr::Unsafe(_))
}

fn extract_unsafe_exprs(block: &Block) -> Vec<Expr> {
    block
        .stmts
        .iter()
        .filter_map(|stmt| match stmt {
            Stmt::Expr(expr, _) | Stmt::Semi(expr, _) => Some(expr),
            _ => None,
        })
        .filter(|expr| is_unsafe_expr(expr))
        .cloned()
        .collect()
}

fn extract_arithmetic_exprs(expr: &Expr) -> Vec<Expr> {
    let mut exprs = Vec::new();
    match expr {
        Expr::Binary(expr_binary) => {
            if matches!(
                expr_binary.op,
                BinOp::Add(_)
                    | BinOp::Sub(_)
                    | BinOp::Mul(_)
                    | BinOp::Div(_)
                    | BinOp::Rem(_)
            ) {
                exprs.push(expr.clone());
            }
            exprs.extend(extract_arithmetic_exprs(&expr_binary.left));
            exprs.extend(extract_arithmetic_exprs(&expr_binary.right));
        }
        Expr::Block(expr_block) => {
            for stmt in &expr_block.block.stmts {
                match stmt {
                    Stmt::Expr(inner_expr, _) | Stmt::Semi(inner_expr, _) => {
                        exprs.extend(extract_arithmetic_exprs(inner_expr));
                    }
                    _ => {}
                }
            }
        }
        Expr::Paren(expr_paren) => {
            exprs.extend(extract_arithmetic_exprs(&expr_paren.expr));
        }
        Expr::Unary(expr_unary) => {
            exprs.extend(extract_arithmetic_exprs(&expr_unary.expr));
        }
        _ => {}
    }
    exprs
}

fn find_unsafe_arithmetic_exprs(block: &Block) -> Vec<Expr> {
    let unsafe_exprs = extract_unsafe_exprs(block);
    let mut arithmetic_exprs = Vec::new();
    for unsafe_expr in unsafe_exprs {
        arithmetic_exprs.extend(extract_arithmetic_exprs(&unsafe_expr));
    }
    arithmetic_exprs
}

fn extract_variable_names(expr: &Expr) -> Vec<Ident> {
    let mut names = Vec::new();
    match expr {
        Expr::Path(expr_path) => {
            if expr_path.path.segments.len() == 1 {
                names.push(expr_path.path.segments[0].ident.clone());
            }
        }
        Expr::Binary(expr_binary) => {
            names.extend(extract_variable_names(&expr_binary.left));
            names.extend(extract_variable_names(&expr_binary.right));
        }
        Expr::Unary(expr_unary) => {
            names.extend(extract_variable_names(&expr_unary.expr));
        }
        Expr::Paren(expr_paren) => {
            names.extend(extract_variable_names(&expr_paren.expr));
        }
        _ => {}
    }
    names
}

fn generate_float_expr() -> Expr {
    parse_quote! { 3.14 }
}

fn generate_float_local() -> Local {
    parse_quote! { let c = 3.14; }
}

fn replace_expr_with_float(expr: &Expr) -> Expr {
    let mut new_expr = expr.clone();
    if let Expr::Binary(expr_binary) = &mut new_expr {
        let left_names = extract_variable_names(&expr_binary.left);
        let right_names = extract_variable_names(&expr_binary.right);
        if !left_names.is_empty() && !right_names.is_empty() {
            expr_binary.right = Box::new(generate_float_expr());
        }
    }
    new_expr
}

fn replace_arithmetic_exprs_with_float(exprs: &[Expr], block: &mut Block) {
    let mut replaced_exprs = Vec::new();
    for expr in exprs {
        replaced_exprs.push(replace_expr_with_float(expr));
    }
    let mut new_stmts = Vec::new();
    for stmt in &block.stmts {
        match stmt {
            Stmt::Expr(expr, semi) => {
                if exprs.contains(expr) {
                    for (i, original_expr) in exprs.iter().enumerate() {
                        if original_expr == expr {
                            new_stmts.push(Stmt::Expr(replaced_exprs[i].clone(), *semi));
                        }
                    }
                } else {
                    new_stmts.push(stmt.clone());
                }
            }
            Stmt::Semi(expr, semi) => {
                if exprs.contains(expr) {
                    for (i, original_expr) in exprs.iter().enumerate() {
                        if original_expr == expr {
                            new_stmts.push(Stmt::Semi(replaced_exprs[i].clone(), *semi));
                        }
                    }
                } else {
                    new_stmts.push(stmt.clone());
                }
            }
            _ => {
                new_stmts.push(stmt.clone());
            }
        }
    }
    block.stmts = new_stmts;
}

fn inject_float_local(block: &mut Block) {
    let float_local = generate_float_local();
    block.stmts.insert(0, Stmt::Local(float_local));
}

pub struct Undefined_Binary_Operation_In_Unsafe_Block_56;

impl Mutator for Undefined_Binary_Operation_In_Unsafe_Block_56 {
    fn name(&self) -> &str {
        "Undefined_Binary_Operation_In_Unsafe_Block_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let arithmetic_exprs = find_unsafe_arithmetic_exprs(&item_fn.block);
                if !arithmetic_exprs.is_empty() {
                    replace_arithmetic_exprs_with_float(&arithmetic_exprs, &mut item_fn.block);
                    inject_float_local(&mut item_fn.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets arithmetic operations within unsafe blocks by replacing one operand with a floating-point literal, specifically 3.14. It first identifies all arithmetic expressions within unsafe blocks, then substitutes one operand with a float, and finally injects a new local variable declaration for the float. This transformation aims to introduce undefined or incompatible binary operations, leveraging Rust's relaxed safety checks in unsafe blocks to stress-test the compiler's type system and error handling."
    }
}