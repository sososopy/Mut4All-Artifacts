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

pub struct Attribute_Misplacement_25;

impl Mutator for Attribute_Misplacement_25 {
    fn name(&self) -> &str {
        "Attribute_Misplacement_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut misplaced_attributes = Vec::new();
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Unary(expr_unary) = &*init.expr {
                                if let Expr::Path(expr_path) = &*expr_unary.expr {
                                    if !expr_path.attrs.is_empty() {
                                        misplaced_attributes.extend(expr_path.attrs.clone());
                                        expr_path.attrs = Vec::new();
                                    }
                                }
                            }
                        }
                    }
                }
                if !misplaced_attributes.is_empty() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if local.attrs.is_empty() {
                                local.attrs.append(&mut misplaced_attributes);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets attributes applied to expressions within function bodies. It removes these attributes from their original locations and misplaces them onto other statements, such as `let` bindings, where they are not typically valid. This transformation is designed to test the compiler's handling of misplaced attributes and may lead to parser errors or unexpected behavior."
    }
}