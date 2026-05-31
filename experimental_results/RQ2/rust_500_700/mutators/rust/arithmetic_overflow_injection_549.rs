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

pub struct Arithmetic_Overflow_Injection_549;

impl Mutator for Arithmetic_Overflow_Injection_549 {
    fn name(&self) -> &str {
        "Arithmetic_Overflow_Injection_549"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Binary(expr_binary), _) = stmt {
                        let large_constant: Expr = parse_quote! { 10_000_000_000 };
                        match expr_binary.op {
                            syn::BinOp::Add(_) | syn::BinOp::Sub(_) | syn::BinOp::Mul(_) => {
                                expr_binary.right = Box::new(Expr::Binary(syn::ExprBinary {
                                    attrs: vec![],
                                    left: expr_binary.right.clone(),
                                    op: syn::BinOp::Add(Default::default()),
                                    right: Box::new(large_constant.clone()),
                                }));
                            }
                            syn::BinOp::Div(_) => {
                                expr_binary.right = Box::new(Expr::Binary(syn::ExprBinary {
                                    attrs: vec![],
                                    left: expr_binary.right.clone(),
                                    op: syn::BinOp::Div(Default::default()),
                                    right: Box::new(parse_quote! { 0.0001 }),
                                }));
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets arithmetic operations to inject potential overflow scenarios. By modifying expressions to include large constants or divisors close to zero, it tests the program's handling of overflow and division by small values, potentially exposing bugs related to arithmetic operations."
    }
}