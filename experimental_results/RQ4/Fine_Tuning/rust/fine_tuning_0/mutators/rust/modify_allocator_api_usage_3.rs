use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Allocator_Api_Usage_3;

impl Mutator for Modify_Allocator_Api_Usage_3 {
    fn name(&self) -> &str {
        "Modify_Allocator_Api_Usage_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::ForLoop(for_loop), _) = stmt {
                        if let Expr::MethodCall(method_call) = &*for_loop.expr {
                            if method_call.method == "iter_mut" {
                                let mut new_stmts = Vec::new();
                                for stmt in &for_loop.body.stmts {
                                    if let Stmt::Expr(Expr::Assign(assign), _) = stmt {
                                        if let Expr::Field(field) = &*assign.left {
                                            if let Expr::Path(ExprPath { path, .. }) = &*field.base
                                            {
                                                if path.is_ident("MaybeUninit") {
                                                    let new_stmt: Stmt = parse_quote! {
                                                        if i % 2 == 0 {
                                                            *a = MaybeUninit::new(f(i));
                                                        } else {
                                                            *a = MaybeUninit::new(default_value());
                                                        }
                                                    };
                                                    new_stmts.push(new_stmt);
                                                    continue;
                                                }
                                            }
                                        }
                                    }
                                    new_stmts.push(stmt.clone());
                                }
                                for_loop.body.stmts = new_stmts;
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