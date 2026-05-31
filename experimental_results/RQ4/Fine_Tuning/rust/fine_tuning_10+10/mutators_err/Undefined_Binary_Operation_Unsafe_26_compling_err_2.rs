use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Undefined_Binary_Operation_Unsafe_26;

impl Mutator for Undefined_Binary_Operation_Unsafe_26 {
    fn name(&self) -> &str {
        "Undefined_Binary_Operation_Unsafe_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ItemFn { block, .. }) = item {
                let mut new_stmts = Vec::new();
                for stmt in &block.stmts {
                    if let Stmt::Expr(Expr::Unsafe(expr_unsafe), _) = stmt {
                        for inner_stmt in &expr_unsafe.block.stmts {
                            if let Stmt::Local(Local { pat, init: Some(LocalInit { expr, .. }), .. }) = inner_stmt {
                                if let Expr::Binary(expr_binary) = &**expr {
                                    let left_type = infer_type(&expr_binary.left);
                                    let right_type = infer_type(&expr_binary.right);
                                    if is_integer_type(&left_type) && is_integer_type(&right_type) {
                                        let new_var_ident = Ident::new("c", Span::call_site());
                                        let new_var_type = parse_quote!(f64);
                                        let new_var_value = parse_quote!(3.14);
                                        let new_var = Stmt::Local(parse_quote!(let #new_var_ident: #new_var_type = #new_var_value;));
                                        new_stmts.push(new_var);
                                        let new_expr = Expr::Binary(syn::ExprBinary {
                                            attrs: vec![],
                                            left: expr_binary.left.clone(),
                                            op: expr_binary.op.clone(),
                                            right: Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                qself: None,
                                                path: SynPath::from(new_var_ident),
                                            })),
                                        });
                                        let new_stmt = Stmt::Expr(new_expr, None);
                                        new_stmts.push(new_stmt);
                                        continue;
                                    }
                                }
                            }
                            new_stmts.push(inner_stmt.clone());
                        }
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies arithmetic operations within unsafe blocks and alters them by introducing operands of incompatible types, such as mixing integers with floating-point numbers. This is achieved by adding new variables with explicit types and modifying existing operations to use these variables, potentially exposing issues in type handling within unsafe contexts."
    }
}

fn infer_type(expr: &Expr) -> Type {
    match expr {
        Expr::Lit(expr_lit) => match &expr_lit.lit {
            syn::Lit::Int(_) => parse_quote!(i32),
            syn::Lit::Float(_) => parse_quote!(f32),
            _ => parse_quote!(()),
        },
        _ => parse_quote!(()),
    }
}

fn is_integer_type(ty: &Type) -> bool {
    matches!(ty, Type::Path(TypePath { path, .. }) if path.is_ident("i32") || path.is_ident("u32"))
}