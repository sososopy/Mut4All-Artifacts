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

pub struct Recursive_Call_With_Modified_Args_148;

impl Mutator for Recursive_Call_With_Modified_Args_148 {
    fn name(&self) -> &str {
        "Recursive_Call_With_Modified_Args_148"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    let mut has_recursive_call = false;
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(Expr::Call(ExprCall { func: expr_func, .. }), _) = stmt {
                            if let Expr::Path(ExprPath { path, .. }) = &**expr_func {
                                if path.is_ident(&func.sig.ident) {
                                    has_recursive_call = true;
                                    break;
                                }
                            }
                        }
                    }
                    if !has_recursive_call {
                        let new_stmt: Stmt = parse_quote! {
                            {
                                let arr = vec![1, 2, 3];
                                let i = arr.len(); // potentially out-of-bounds index
                                #func.sig.ident(arr[i]);
                            }
                        };
                        func.block.stmts.push(new_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets the main function, introducing a recursive call with modified arguments. It adds a vector and uses an index that is potentially out-of-bounds, aiming to test the compiler's handling of recursion and indexing."
    }
}