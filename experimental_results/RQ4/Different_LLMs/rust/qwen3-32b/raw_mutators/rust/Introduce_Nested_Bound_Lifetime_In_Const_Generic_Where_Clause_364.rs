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

pub struct Introduce_Nested_Bound_Lifetime_In_Const_Generic_Where_Clause_364;

impl Mutator for Introduce_Nested_Bound_Lifetime_In_Const_Generic_Where_Clause_364 {
    fn name(&self) -> &str {
        "Introduce_Nested_Bound_Lifetime_In_Const_Generic_Where_Clause_364"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred) = predicate {
                            if let syn::Type::Array(array) = &*pred.bounded_ty {
                                let lifetime = syn::Lifetime::new("'a", Span::call_site());
                                let new_len = parse_quote!({
                                    let y: &'a u8;
                                    0
                                });
                                let new_array_type = syn::Type::Array(syn::TypeArray {
                                    bracket_token: array.bracket_token.clone(),
                                    elem: array.elem.clone(),
                                    semi_token: array.semi_token.clone(),
                                    len: Box::new(new_len),
                                });
                                let new_bounds = Punctuated::new();
                                let new_type_pred = syn::WherePredicate::Type(syn::PredicateType {
                                    lifetimes: None,
                                    bounded_ty: Box::new(new_array_type),
                                    colon_token: Default::default(),
                                    bounds: new_bounds,
                                });
                                let new_for_pred = syn::WherePredicate::For(syn::PredicateFor {
                                    for_token: Default::default(),
                                    bound_lifetimes: BoundLifetimes {
                                        lifetimes: vec![lifetime].into_iter().collect(),
                                    },
                                    predicate: Box::new(new_type_pred),
                                });
                                *predicate = new_for_pred;
                                break;
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