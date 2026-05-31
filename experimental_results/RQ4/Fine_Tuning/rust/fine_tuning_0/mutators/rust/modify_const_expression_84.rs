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

pub struct Modify_Const_Expression_84;

impl Mutator for Modify_Const_Expression_84 {
    fn name(&self) -> &str {
        "Modify_Const_Expression_84"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(const_param) = func.sig.generics.params.iter().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some(const_param.ident.clone())
                    } else {
                        None
                    }
                }) {
                    let mut rng = thread_rng();
                    let new_operator = if rng.gen_bool(0.5) { '+' } else { '-' };
                    
                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Array(type_array) = &mut **return_type {
                            if let Expr::Binary(expr_binary) = &mut type_array.len {
                                if let Expr::Path(expr_path) = &*expr_binary.left {
                                    if expr_path.path.is_ident(&const_param) {
                                        expr_binary.op = syn::BinOp::Add(token::Plus { spans: [Span::call_site()] });
                                        if let Expr::Lit(expr_lit) = &*expr_binary.right {
                                            if let syn::Lit::Int(lit_int) = expr_lit.lit.clone() {
                                                let value = lit_int.base10_parse::<usize>().unwrap();
                                                let new_value = if new_operator == '+' {
                                                    value + 1
                                                } else {
                                                    value.saturating_sub(1)
                                                };
                                                expr_binary.right = Box::new(parse_quote!(#new_value));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                if let Type::Array(type_array) = &mut predicate_type.bounded_ty {
                                    if let Expr::Binary(expr_binary) = &mut type_array.len {
                                        if let Expr::Path(expr_path) = &*expr_binary.left {
                                            if expr_path.path.is_ident(&const_param) {
                                                expr_binary.op = syn::BinOp::Add(token::Plus { spans: [Span::call_site()] });
                                                if let Expr::Lit(expr_lit) = &*expr_binary.right {
                                                    if let syn::Lit::Int(lit_int) = expr_lit.lit.clone() {
                                                        let value = lit_int.base10_parse::<usize>().unwrap();
                                                        let new_value = if new_operator == '+' {
                                                            value + 1
                                                        } else {
                                                            value.saturating_sub(1)
                                                        };
                                                        expr_binary.right = Box::new(parse_quote!(#new_value));
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
        "The mutation operator identifies functions with const generic parameters and modifies arithmetic operations involving these parameters. It changes the operator or adjusts the constant value in expressions, testing the compiler's ability to handle variations in const expressions within function signatures and where clauses."
    }
}