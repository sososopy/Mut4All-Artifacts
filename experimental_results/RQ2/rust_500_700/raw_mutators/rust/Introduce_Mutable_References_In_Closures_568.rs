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

pub struct Introduce_Mutable_References_In_Closures_568;

impl Mutator for Introduce_Mutable_References_In_Closures_568 {
    fn name(&self) -> &str {
        "Introduce_Mutable_References_In_Closures_568"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let syn::Local {
                            init: Some((_, expr)),
                            ..
                        } = local
                        {
                            if let syn::Expr::Closure(closure) = &mut **expr {
                                let mut captured_vars = Vec::new();
                                closure.inputs.iter().for_each(|arg| {
                                    if let syn::Pat::Ident(pat_ident) = arg {
                                        captured_vars.push(pat_ident.ident.clone());
                                    }
                                });
                                if !captured_vars.is_empty() {
                                    let mut stmts = Vec::new();
                                    for var in captured_vars {
                                        let mutable_ident = Ident::new(
                                            &format!("{}_mutable", var),
                                            Span::call_site(),
                                        );
                                        let mutable_stmt: Stmt = parse_quote! {
                                            let mut #mutable_ident = #var;
                                        };
                                        stmts.push(mutable_stmt);
                                    }
                                    stmts.push(Stmt::Expr(expr.clone()));
                                    *stmt = Stmt::Block(syn::Block {
                                        brace_token: syn::token::Brace::default(),
                                        stmts,
                                    });
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