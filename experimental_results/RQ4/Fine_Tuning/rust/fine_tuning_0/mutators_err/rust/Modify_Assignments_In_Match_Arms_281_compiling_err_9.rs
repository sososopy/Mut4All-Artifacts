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

pub struct Modify_Assignments_In_Match_Arms_281;

impl Mutator for Modify_Assignments_In_Match_Arms_281 {
    fn name(&self) -> &str {
        "Modify_Assignments_In_Match_Arms_281"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut match_exprs = Vec::new();

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let stmts = &mut item_fn.block.stmts;
                for stmt in stmts.iter_mut() {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::Path(expr_path) = &*init.expr {
                                if let Some(ident) = expr_path.path.get_ident() {
                                    if ident == "Some" || ident == "None" {
                                        if let Pat::Ident(pat_ident) = &local.pat {
                                            let var_name = &pat_ident.ident;
                                            for stmt in stmts.iter() {
                                                if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                                                    if let Expr::Path(match_expr_path) = &*expr_match.expr {
                                                        if match_expr_path.path.get_ident() == Some(var_name) {
                                                            match_exprs.push(expr_match.clone());
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

                for expr_match in &mut match_exprs {
                    for arm in &mut expr_match.arms {
                        if let Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                            if let Some(path_segment) = pat_tuple_struct.path.segments.first() {
                                if path_segment.ident == "Some" {
                                    if let Pat::Ident(pat_ident) = &pat_tuple_struct.elems.first().unwrap() {
                                        let arm_var_name = &pat_ident.ident;
                                        let reassignment: Stmt = parse_quote! { #arm_var_name = None; };
                                        if let Expr::Block(expr_block) = &mut *arm.body {
                                            expr_block.block.stmts.insert(0, reassignment);
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
        "This mutation operator modifies match expressions that pattern match on Option types. It targets the Some(variable) arm and introduces a reassignment to the matched variable, causing potential ambiguity in the variable's usage. This transformation aims to explore scenarios that might lead to inconsistencies or unexpected behaviors in the type system, potentially uncovering compiler bugs related to improper handling of variable reassignments in match expressions."
    }
}