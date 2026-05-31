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

pub struct Modify_Macro_Invocation_Context_22;

impl Mutator for Modify_Macro_Invocation_Context_22 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_Context_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Macro(expr_macro)) = stmt {
                        let condition: Expr = parse_quote! { true };
                        let wrapped_stmt: Stmt = parse_quote! {
                            if #condition {
                                #stmt
                            }
                        };
                        new_stmts.push(wrapped_stmt);
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}