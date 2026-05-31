use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Introduce_Mutable_Destructuring_224;

impl Mutator for Introduce_Mutable_Destructuring_224 {
    fn name(&self) -> &str {
        "Introduce_Mutable_Destructuring_224"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_stmts = Vec::new();
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Pat::TupleStruct(pat_tuple_struct) = &local.pat {
                                if let Expr::Call(call_expr) = &*init.expr {
                                    if let Expr::Path(path_expr) = &*call_expr.func {
                                        if path_expr.path.segments.last().unwrap().ident == "default" {
                                            if let Pat::TupleStruct(pat_tuple_struct) = &local.pat {
                                                let struct_name = &pat_tuple_struct.path;
                                                let mut temp_var_name = Ident::new("temp_var", Span::call_site());
                                                let mut counter = 0;
                                                while item_fn.block.stmts.iter().any(|s| {
                                                    if let Stmt::Local(l) = s {
                                                        if let Pat::Ident(pat_ident) = &l.pat {
                                                            pat_ident.ident == temp_var_name
                                                        } else {
                                                            false
                                                        }
                                                    } else {
                                                        false
                                                    }
                                                }) {
                                                    counter += 1;
                                                    temp_var_name = Ident::new(&format!("temp_var_{}", counter), Span::call_site());
                                                }
                                                let new_local: Stmt = parse_quote! {
                                                    let mut #temp_var_name = Default::default();
                                                };
                                                new_stmts.push(new_local);
                                                let new_stmt: Stmt = parse_quote! {
                                                    #struct_name([..]) = { let _ = &mut #temp_var_name; #temp_var_name };
                                                };
                                                new_stmts.push(new_stmt);
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies destructuring assignments of structs with array patterns where the right-hand side is a call to Default::default(). It introduces a new mutable variable initialized to Default::default() before the original statement, then modifies the original statement to use a mutable borrow of that variable. This transformation increases MIR complexity by adding intermediate mutable bindings and borrows, potentially stressing destructuring and inlining optimizations."
    }
}