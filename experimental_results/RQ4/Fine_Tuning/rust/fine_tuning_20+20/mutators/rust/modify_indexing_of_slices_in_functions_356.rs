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

pub struct Modify_Indexing_Of_Slices_In_Functions_356;

impl Mutator for Modify_Indexing_Of_Slices_In_Functions_356 {
    fn name(&self) -> &str {
        "Modify_Indexing_Of_Slices_In_Functions_356"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_slice_access = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::MethodCall(method_call) = expr {
                            if method_call.method == "copy_from_slice" {
                                has_slice_access = true;
                                break;
                            }
                        }
                    }
                }
                if has_slice_access {
                    let mut new_stmts = Vec::new();
                    for stmt in &item_fn.block.stmts {
                        match stmt {
                            Stmt::Expr(expr, semi) => {
                                if let Expr::MethodCall(method_call) = expr {
                                    if method_call.method == "copy_from_slice" {
                                        if let Some(arg) = method_call.args.first() {
                                            if let Expr::Reference(ExprReference {
                                                expr: inner_expr,
                                                ..
                                            }) = arg
                                            {
                                                if let Expr::Index(ExprIndex {
                                                    expr: base_expr,
                                                    ..
                                                }) = &**inner_expr
                                                {
                                                    if let Expr::Field(ExprField {
                                                        base, ..
                                                    }) = &**base_expr
                                                    {
                                                        let new_expr: Expr = parse_quote! {
                                                            #base[..#base.len() + 1]
                                                        };
                                                        let new_method_call: Expr =
                                                            parse_quote! {
                                                                buf.copy_from_slice(&#new_expr)
                                                            };
                                                        new_stmts.push(Stmt::Expr(
                                                            new_method_call,
                                                            semi.clone(),
                                                        ));
                                                        continue;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                new_stmts.push(stmt.clone());
                            }
                            _ => new_stmts.push(stmt.clone()),
                        }
                    }
                    item_fn.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that access slices, specifically those using `copy_from_slice`. It identifies such functions and modifies the slice indexing to use an out-of-bounds index, one more than the slice's maximum index. This transformation is designed to induce runtime panics by exploiting slice bounds, aiming to test the compiler's robustness in handling indexing and slice access in the presence of potentially incomplete features like `generic_const_exprs`."
    }
}