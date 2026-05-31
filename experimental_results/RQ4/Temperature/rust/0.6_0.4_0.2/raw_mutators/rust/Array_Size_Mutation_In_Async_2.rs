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

pub struct Array_Size_Mutation_In_Async_2;

impl Mutator for Array_Size_Mutation_In_Async_2 {
    fn name(&self) -> &str {
        "Array_Size_Mutation_In_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, init)) = &local.init {
                                if let Expr::Array(expr_array) = &**init {
                                    if let Expr::Repeat(expr_repeat) = &**expr_array {
                                        if let Expr::Lit(expr_lit) = &*expr_repeat.len {
                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                if let Ok(size) = lit_int.base10_parse::<u128>() {
                                                    if size > 1000 {
                                                        let new_size: Expr = parse_quote! { 100 };
                                                        let new_repeat_expr = Expr::Repeat(syn::ExprRepeat {
                                                            bracket_token: expr_repeat.bracket_token,
                                                            expr: expr_repeat.expr.clone(),
                                                            len: Box::new(new_size),
                                                        });
                                                        *init = Box::new(Expr::Array(syn::ExprArray {
                                                            bracket_token: expr_array.bracket_token,
                                                            elems: parse_quote! { #new_repeat_expr },
                                                        }));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}