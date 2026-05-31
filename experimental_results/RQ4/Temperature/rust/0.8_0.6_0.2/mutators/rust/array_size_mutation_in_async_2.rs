use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprPath, ExprRepeat, File, FnArg, GenericArgument, GenericParam, Ident,
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
                            if let Some(init) = &local.init {
                                if let Expr::Array(expr_array) = &*init.expr {
                                    if let Expr::Repeat(expr_repeat) = &expr_array.elems[0] {
                                        if let Expr::Lit(expr_lit) = &*expr_repeat.len {
                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                let original_size = lit_int.base10_parse::<usize>().unwrap();
                                                let new_sizes = vec![0, 1, usize::MAX];
                                                let mut rng = thread_rng();
                                                let new_size = new_sizes.choose(&mut rng).unwrap();
                                                let new_size_expr: Expr = parse_quote!(*new_size);
                                                let new_len = Box::new(new_size_expr);
                                                let expr_repeat_mut = expr_repeat as *const ExprRepeat as *mut ExprRepeat;
                                                unsafe {
                                                    (*expr_repeat_mut).len = new_len;
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