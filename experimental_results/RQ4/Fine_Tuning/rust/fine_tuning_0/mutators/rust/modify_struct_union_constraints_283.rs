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

pub struct Modify_Struct_Union_Constraints_283;

impl Mutator for Modify_Struct_Union_Constraints_283 {
    fn name(&self) -> &str {
        "Modify_Struct_Union_Constraints_283"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(where_clause) = &mut item_struct.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                if let Type::Array(type_array) = &predicate_type.bounded_ty {
                                    if let Expr::Binary(expr_binary) = &type_array.len {
                                        if let syn::BinOp::Add(_) = expr_binary.op {
                                            let new_expr: Expr = parse_quote! { #expr_binary.left * #expr_binary.right };
                                            predicate_type.bounded_ty = Type::Array(TypeArray {
                                                len: new_expr,
                                                ..type_array.clone()
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Union(item_union) => {
                    if let Some(where_clause) = &mut item_union.generics.where_clause {
                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                if let Type::Array(type_array) = &predicate_type.bounded_ty {
                                    if let Expr::Binary(expr_binary) = &type_array.len {
                                        if let syn::BinOp::Add(_) = expr_binary.op {
                                            let new_expr: Expr = parse_quote! { #expr_binary.left * #expr_binary.right };
                                            predicate_type.bounded_ty = Type::Array(TypeArray {
                                                len: new_expr,
                                                ..type_array.clone()
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}