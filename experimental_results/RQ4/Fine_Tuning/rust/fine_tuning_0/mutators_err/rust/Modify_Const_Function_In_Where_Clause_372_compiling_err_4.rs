use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Modify_Const_Function_In_Where_Clause_372;

impl Mutator for Modify_Const_Function_In_Where_Clause_372 {
    fn name(&self) -> &str {
        "Modify_Const_Function_In_Where_Clause_372"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "usize" {
                                        if let Some(const_expr) = find_generic_const_expr(&segment.arguments) {
                                            if let Expr::Block(expr_block) = const_expr {
                                                if let Some(Stmt::Expr(Expr::Call(expr_call), _)) = expr_block.block.stmts.first() {
                                                    if let Expr::Path(expr_path) = &*expr_call.func {
                                                        if let Some(last_segment) = expr_path.path.segments.last() {
                                                            if last_segment.ident == "inner" {
                                                                let new_expr: Expr = parse_quote! { false };
                                                                let new_block: ExprBlock = parse_quote! {
                                                                    {
                                                                        const fn inner() -> bool { true }
                                                                        inner();
                                                                        false
                                                                    }
                                                                };
                                                                *expr_block = new_block;
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with a `where` clause that uses a generic constant expression. It identifies the const function within the expression and modifies its body to return a different constant type. This changes the surrounding context to incorporate the new type, testing the compiler's handling of const generics and type inference."
    }
}

fn find_generic_const_expr(arguments: &PathArguments) -> Option<&mut Expr> {
    if let PathArguments::AngleBracketed(angle_bracketed) = arguments {
        for arg in &mut angle_bracketed.args {
            if let GenericArgument::Const(expr) = arg {
                return Some(expr);
            }
        }
    }
    None
}