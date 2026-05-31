use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprBinary, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
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

pub struct Replace_Generic_Subtraction_With_Underflow_112;

impl Mutator for Replace_Generic_Subtraction_With_Underflow_112 {
    fn name(&self) -> &str {
        "Replace_Generic_Subtraction_With_Underflow_112"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred) = predicate {
                            if let Type::Array(arr) = &mut *pred.bounded_ty {
                                if let Expr::Block(expr_block) = &mut *arr.len {
                                    if let Expr::Binary(expr_binary) = &mut *expr_block.block {
                                        if let syn::BinOp::Sub(_) = expr_binary.op {
                                            *expr_binary.right = syn::parse_quote! { 1 };
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(pred) = predicate {
                                    if let Type::Array(arr) = &mut *pred.bounded_ty {
                                        if let Expr::Block(expr_block) = &mut *arr.len {
                                            if let Expr::Binary(expr_binary) = &mut *expr_block.block {
                                                if let syn::BinOp::Sub(_) = expr_binary.op {
                                                    *expr_binary.right = syn::parse_quote! { 1 };
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
        "The mutation operator targets const generic subtractions in where clauses, replacing the right operand with 1 to induce underflow when the left operand is zero. This transformation aims to trigger overflow checks during const evaluation, testing the compiler's handling of arithmetic underflow in generic constraints."
    }
}