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

pub struct Adjust_Array_Size_In_Async_2;

impl Mutator for Adjust_Array_Size_In_Async_2 {
    fn name(&self) -> &str {
        "Adjust_Array_Size_In_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &mut local.init {
                                if let Expr::Array(expr_array) = &mut *init.expr {
                                    if let Some(Expr::Repeat(expr_repeat)) = expr_array.elems.first_mut() {
                                        if let Expr::Lit(expr_lit) = &mut *expr_repeat.len {
                                            if let syn::Lit::Int(_) = &expr_lit.lit {
                                                let new_size = parse_quote!(usize::MAX);
                                                expr_repeat.len = Box::new(Expr::Lit(ExprLit {
                                                    attrs: vec![],
                                                    lit: syn::Lit::Int(new_size),
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array declarations within async functions. It modifies the size of arrays to `usize::MAX`, aiming to expose potential compiler bugs related to memory management or integer arithmetic in async contexts. By altering the array size to a value that could lead to overflow, the transformation stresses the compiler's handling of large data structures."
    }
}