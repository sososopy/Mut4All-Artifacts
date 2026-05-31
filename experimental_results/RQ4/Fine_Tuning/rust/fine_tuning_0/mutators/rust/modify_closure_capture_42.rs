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

pub struct Modify_Closure_Capture_42;

impl Mutator for Modify_Closure_Capture_42 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let mut closure_expr = None;
                        let mut closure_index = None;
                        for (index, stmt) in item_fn.block.stmts.iter().enumerate() {
                            if let Stmt::Expr(Expr::Closure(expr_closure), _) = stmt {
                                closure_expr = Some(expr_closure.clone());
                                closure_index = Some(index);
                                break;
                            }
                        }

                        if let Some(expr_closure) = closure_expr {
                            let additional_var: Stmt = parse_quote! {
                                let additional_value = vec![1, 2, 3];
                            };
                            item_fn.block.stmts.insert(0, additional_var);

                            let mut new_closure = expr_closure.clone();
                            if let Expr::Block(expr_block) = &mut *new_closure.body {
                                let capture_stmt: Stmt = parse_quote! {
                                    let _ = additional_value.len();
                                };
                                expr_block.block.stmts.insert(0, capture_stmt);
                            }

                            if let Some(index) = closure_index {
                                if let Stmt::Expr(Expr::Closure(expr_closure), _) = &mut item_fn.block.stmts[index] {
                                    *expr_closure = new_closure;
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