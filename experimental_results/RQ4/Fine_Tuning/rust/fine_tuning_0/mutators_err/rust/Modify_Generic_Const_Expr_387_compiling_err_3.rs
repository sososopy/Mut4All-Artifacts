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

pub struct Modify_Generic_Const_Expr_387;

impl Mutator for Modify_Generic_Const_Expr_387 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expr_387"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    if let syn::PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                                        for arg in &mut angle_args.args {
                                            if let syn::GenericArgument::Const(expr) = arg {
                                                if let syn::Expr::Block(expr_block) = expr {
                                                    let new_expr: syn::Expr = parse_quote! { X * 2 };
                                                    expr_block.block.stmts.clear();
                                                    expr_block.block.stmts.push(syn::Stmt::Expr(new_expr));
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
        "This mutator targets function definitions with a `where` clause containing const generic expressions. It modifies the const expression to introduce a non-linear operation, potentially causing instability in evaluation and triggering bugs related to the `generic_const_exprs` feature."
    }
}