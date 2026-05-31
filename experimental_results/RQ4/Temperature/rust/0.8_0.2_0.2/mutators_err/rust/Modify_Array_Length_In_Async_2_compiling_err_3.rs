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

pub struct Modify_Array_Length_In_Async_2;

impl Mutator for Modify_Array_Length_In_Async_2 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let syn::Stmt::Expr(expr, _) = stmt {
                            if let syn::Expr::Await(expr_await) = expr {
                                if let syn::Expr::Array(expr_array) = &*expr_await.base {
                                    if let syn::Expr::Repeat(expr_repeat) = &expr_array.elems[0] {
                                        if let syn::Expr::Lit(expr_lit) = &*expr_repeat.len {
                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                let new_len: Expr = parse_quote!(0xffffffff);
                                                expr_repeat.len = Box::new(new_len);
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
        "The mutation operator targets arrays within async functions that are awaited. It modifies the array length to a large constant value, such as `0xffffffff`, to test the compiler's handling of potential overflow or range-related errors in async contexts. This aims to expose bugs related to extreme conditions in async-await constructs."
    }
}