use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprAwait, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Modify_Array_Size_Await_2;

impl Mutator for Modify_Array_Size_Await_2 {
    fn name(&self) -> &str {
        "Modify_Array_Size_Await_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if !func.sig.asyncness.is_some() {
                    continue;
                }

                let mut mutated_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Await(expr_await)) = stmt {
                        if let Expr::Array(expr_array) = &*expr_await.base {
                            if let Expr::Lit(expr_lit) = &expr_array.elems[1] {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    let original_size: usize = lit_int.base10_parse().unwrap_or(0);
                                    let sizes = vec![0, 1, u32::MAX as usize, original_size * 2];
                                    for &new_size in &sizes {
                                        let new_expr: Expr = parse_quote! {
                                            [0usize; #new_size].await
                                        };
                                        mutated_stmts.push(Stmt::Expr(new_expr));
                                    }
                                    continue;
                                }
                            }
                        }
                    }
                    mutated_stmts.push(stmt.clone());
                }
                func.block.stmts = mutated_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}