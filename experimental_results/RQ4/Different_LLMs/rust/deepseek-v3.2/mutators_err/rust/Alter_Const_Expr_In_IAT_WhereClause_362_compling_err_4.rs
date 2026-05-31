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

pub struct Alter_Const_Expr_In_IAT_WhereClause_362;

impl Mutator for Alter_Const_Expr_In_IAT_WhereClause_362 {
    fn name(&self) -> &str {
        "Alter_Const_Expr_In_IAT_WhereClause_362"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_const_param = false;
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_param = true;
                        break;
                    }
                }
                if !has_const_param {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        let mut has_const_param_in_assoc = false;
                        for param in &type_item.generics.params {
                            if let GenericParam::Const(_) = param {
                                has_const_param_in_assoc = true;
                                break;
                            }
                        }
                        if !has_const_param_in_assoc {
                            continue;
                        }
                        if type_item.generics.where_clause.is_none() {
                            let where_clause: WhereClause = parse_quote! { where [(); 0]: };
                            type_item.generics.where_clause = Some(where_clause);
                        }
                        if let Some(where_clause) = &mut type_item.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Slice(type_slice) = &pred_type.bounded_ty {
                                        if let Type::Array(type_array) = &*type_slice.elem {
                                            if let Expr::Binary(expr_bin) = &type_array.len {
                                                let mut new_expr = expr_bin.clone();
                                                std::mem::swap(&mut new_expr.left, &mut new_expr.right);
                                                let new_array_type: TypeArray = parse_quote! { [(); #new_expr] };
                                                let new_slice_type: TypeSlice = parse_quote! { [#new_array_type] };
                                                pred_type.bounded_ty = Type::Slice(new_slice_type);
                                            } else if let Expr::Path(expr_path) = &type_array.len {
                                                let new_bin_expr: ExprBinary = parse_quote! { #expr_path + 1 };
                                                let new_array_type: TypeArray = parse_quote! { [(); #new_bin_expr] };
                                                let new_slice_type: TypeSlice = parse_quote! { [#new_array_type] };
                                                pred_type.bounded_ty = Type::Slice(new_slice_type);
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
        "The mutation operator targets inherent associated type definitions within impl blocks that have const generic parameters. It modifies const expressions in where-clauses that involve array length predicates, specifically swapping operands of binary expressions or adding a constant offset to path expressions. This transformation aims to trigger the internal compiler error (ICE) related to const generic parameter substitution when the compiler expects a const but finds a type, by altering the structure of const expressions used in where-clauses of inherent associated types."
    }
}