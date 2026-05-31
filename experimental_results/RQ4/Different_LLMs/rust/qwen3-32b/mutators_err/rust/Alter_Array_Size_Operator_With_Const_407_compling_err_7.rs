use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Alter_Array_Size_Operator_With_Const_407;

impl Mutator for Alter_Array_Size_Operator_With_Const_407 {
    fn name(&self) -> &str {
        "Alter_Array_Size_Operator_With_Const_407"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let const_params: Vec<_> = item_impl
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let syn::GenericParam::Const(param) = param {
                            Some(param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.is_empty() {
                    continue;
                }

                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let &mut syn::Type::Array(ref mut array) = &mut predicate_type.bounded_ty {
                                if let syn::Expr::Binary(expr_bin) = array.len {
                                    if let syn::BinOp::Add(_) = expr_bin.op {
                                        let new_op = syn::BinOp::Sub(syn::token::Minus::default());
                                        let const_name = const_params.first().unwrap();
                                        let new_left = syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: syn::Path::from(const_name.clone()),
                                        });
                                        let new_right = expr_bin.right.clone();
                                        let new_expr = syn::Expr::Binary(syn::ExprBinary {
                                            attrs: expr_bin.attrs.clone(),
                                            left: Box::new(new_left),
                                            op: new_op,
                                            right: new_right,
                                        });
                                        array.len = new_expr;
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
        ""
    }
}