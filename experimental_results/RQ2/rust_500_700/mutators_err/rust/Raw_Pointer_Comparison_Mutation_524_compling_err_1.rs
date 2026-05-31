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

pub struct Raw_Pointer_Comparison_Mutation_524;

impl Mutator for Raw_Pointer_Comparison_Mutation_524 {
    fn name(&self) -> &str {
        "Raw_Pointer_Comparison_Mutation_524"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    let mut has_comparison = false;
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(Expr::Binary(expr_binary)) = stmt {
                            has_comparison = true;
                            let left_expr = &expr_binary.left;
                            let right_expr = &expr_binary.right;
                            let left_ptr: Expr = parse_quote! { &(*#left_expr as *const _) };
                            let right_ptr: Expr = parse_quote! { &(*#right_expr as *const _) };
                            expr_binary.left = Box::new(left_ptr);
                            expr_binary.right = Box::new(right_ptr);
                        }
                    }
                    if !has_comparison {
                        let new_comparison: Stmt = parse_quote! {
                            {
                                let x: i32 = 10;
                                let y: i32 = 20;
                                let x_ptr: *const i32 = &x;
                                let y_ptr: *const i32 = &y;
                                x_ptr < y_ptr
                            }
                        };
                        func.block.stmts.push(new_comparison);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}