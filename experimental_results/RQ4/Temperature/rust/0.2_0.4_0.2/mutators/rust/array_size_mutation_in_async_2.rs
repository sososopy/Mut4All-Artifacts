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

pub struct Array_Size_Mutation_In_Async_2;

impl Mutator for Array_Size_Mutation_In_Async_2 {
    fn name(&self) -> &str {
        "Array_Size_Mutation_In_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(LocalInit { expr, .. }) = &local.init {
                                if let Expr::Array(array) = &**expr {
                                    if let Some(Expr::Lit(expr_lit)) = array.elems.first() {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            let original_size = lit_int.base10_parse::<usize>().unwrap_or(0);
                                            if original_size >= 1000 {
                                                let large_size: Expr = parse_quote! { 0xffff_ffff_ffff_ffff };
                                                let new_array: Expr = parse_quote! { [0u8; #large_size] };
                                                *stmt = Stmt::Local(syn::Local {
                                                    pat: local.pat.clone(),
                                                    init: Some(LocalInit {
                                                        eq_token: token::Eq::default(),
                                                        expr: Box::new(new_array),
                                                        diverge: None,
                                                    }),
                                                    ..local.clone()
                                                });
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
        "The mutation operator targets array initializations within async functions. It modifies the size of arrays with large initial sizes to an extremely large value, aiming to test the compiler's handling of memory allocation and overflow issues in an asynchronous context."
    }
}