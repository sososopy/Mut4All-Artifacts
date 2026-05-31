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

pub struct Insert_SizeOf_Array_For_Unsized_WhereClause_190;

impl Mutator for Insert_SizeOf_Array_For_Unsized_WhereClause_190 {
    fn name(&self) -> &str {
        "Insert_SizeOf_Array_For_Unsized_WhereClause_190"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let has_sized = predicate_type.bounds.iter().any(|bound| {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    trait_bound.path.is_ident("Sized")
                                } else {
                                    false
                                }
                            });
                            if has_sized {
                                let bounded_ty = &predicate_type.bounded_ty;
                                let is_unsized = match bounded_ty {
                                    syn::Type::Reference(_) => true,
                                    syn::Type::Path(type_path) => {
                                        if let Some(segment) = type_path.path.get_ident() {
                                            segment == "str"
                                        } else {
                                            false
                                        }
                                    }
                                    _ => false,
                                };
                                if is_unsized {
                                    let new_expr = parse_quote! {
                                        [(); { let _x: #bounded_ty; std::mem::size_of::<#bounded_ty>() }]
                                    };
                                    let new_stmt = syn::Stmt::Expr(new_expr, None);
                                    func.block.stmts.push(new_stmt);
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