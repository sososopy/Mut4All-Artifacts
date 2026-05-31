use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Const_With_Other_Const_218;

impl Mutator for Replace_Const_With_Other_Const_218 {
    fn name(&self) -> &str {
        "Replace_Const_With_Other_Const_218"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut constants: Vec<Expr> = Vec::new();
        for item in &file.items {
            if let Item::Const(const_item) = item {
                constants.push(*const_item.expr.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Array(expr_array) = expr {
                            for elem in &mut expr_array.elems {
                                if let Expr::Index(expr_index) = elem {
                                    if let Expr::Path(path) = &*expr_index.expr {
                                        if let Some(constant) = constants.iter().find(|c| {
                                            if let Expr::Path(const_path) = c {
                                                const_path.path == path.path
                                            } else {
                                                false
                                            }
                                        }) {
                                            let new_constant = constants
                                                .iter()
                                                .filter(|c| c != &constant)
                                                .choose(&mut thread_rng())
                                                .unwrap()
                                                .clone();
                                            *elem = new_constant;
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
        "The mutation operator replaces constants used in array indexing operations with other constants of the same type but with different values. This transformation aims to test the compiler's ability to handle different constant values in array indexing operations and may lead to bugs related to bounds checking or constant evaluation."
    }
}