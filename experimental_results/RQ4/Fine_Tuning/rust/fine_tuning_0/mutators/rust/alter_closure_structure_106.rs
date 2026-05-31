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

pub struct Alter_Closure_Structure_106;

impl Mutator for Alter_Closure_Structure_106 {
    fn name(&self) -> &str {
        "Alter_Closure_Structure_106"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Closure(closure) = &*init.expr {
                                if closure.inputs.len() > 0 {
                                    if let Expr::Block(block) = &*closure.body {
                                        if block.block.stmts.iter().any(|s| matches!(s, Stmt::Expr(Expr::Unsafe(_), _))) {
                                            let unsafe_block = block.block.stmts.iter().find(|s| matches!(s, Stmt::Expr(Expr::Unsafe(_), _))).unwrap();
                                            let new_expr: Expr = parse_quote! {{
                                                #closure
                                                { #unsafe_block }
                                            }};
                                            local.init = Some(LocalInit {
                                                expr: Box::new(new_expr),
                                                eq_token: Default::default(),
                                                diverge: None,
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies closures containing unsafe blocks and restructures them by moving the unsafe block outside the closure's body. It misplaces closure delimiters to exploit potential compiler handling issues with closures involving unsafe code and parameter binding. This transformation aims to test the compiler's robustness in handling syntax errors related to closure and unsafe block placement."
    }
}