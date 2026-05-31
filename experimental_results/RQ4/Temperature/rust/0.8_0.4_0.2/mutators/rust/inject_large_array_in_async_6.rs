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

pub struct Inject_Large_Array_In_Async_6;

impl Mutator for Inject_Large_Array_In_Async_6 {
    fn name(&self) -> &str {
        "Inject_Large_Array_In_Async_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut has_array = false;
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(Local { init: Some(init), .. }) = stmt {
                            if let Expr::Array(_) = *init.expr {
                                has_array = true;
                                if let Expr::Array(array) = &mut *init.expr {
                                    array.elems = parse_quote!([0u32; 0xffff_ffff_ffff_ffff]);
                                }
                            }
                        }
                    }
                    if !has_array {
                        let new_array: Stmt = parse_quote! {
                            let large_array = [0u32; 0xffff_ffff_ffff_ffff];
                        };
                        item_fn.block.stmts.insert(0, new_array);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets async functions or blocks, injecting or modifying array instantiations to have extremely large sizes. The goal is to test the Rust compiler's handling of potential overflow conditions in async contexts, particularly focusing on memory allocation and size limit management."
    }
}