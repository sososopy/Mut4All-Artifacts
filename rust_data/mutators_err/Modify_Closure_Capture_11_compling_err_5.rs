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

pub struct Modify_Closure_Capture_11;

impl Mutator for Modify_Closure_Capture_11 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_stmts = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = **ty {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Some(LocalInit { expr, .. }) = &local.init {
                                    if let Expr::Closure(closure) = &**expr {
                                        let new_var_ident = Ident::new("additional_value", Span::call_site());
                                        let new_var_type: Type = parse_quote! { Vec<i32> };
                                        let new_var_expr: Expr = parse_quote! { vec![1, 2, 3] };
                                        let new_stmt: Stmt = parse_quote! {
                                            let #new_var_ident: #new_var_type = #new_var_expr;
                                        };
                                        new_stmts.push(new_stmt);

                                        let capture_expr: Expr = parse_quote! {
                                            let _ = #new_var_ident.len();
                                        };
                                        if let Expr::Block(block) = &mut *closure.body {
                                            block.block.stmts.insert(0, Stmt::Expr(capture_expr));
                                        }
                                    }
                                }
                            }
                        }
                        item_fn.block.stmts.splice(0..0, new_stmts.drain(..));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions returning `impl Fn`, locate closures, introduce new variables with different types, modify closures to capture these variables, and ensure closure body references them."
    }
}