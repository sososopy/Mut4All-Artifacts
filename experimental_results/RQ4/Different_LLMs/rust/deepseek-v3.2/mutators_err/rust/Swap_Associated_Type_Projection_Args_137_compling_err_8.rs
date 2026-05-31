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

pub struct Swap_Associated_Type_Projection_Args_137;

impl Mutator for Swap_Associated_Type_Projection_Args_137 {
    fn name(&self) -> &str {
        "Swap_Associated_Type_Projection_Args_137"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    mutate_where_clause_predicates(where_clause, &mut rng);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(where_clause) = &mut impl_item.generics.where_clause {
                    mutate_where_clause_predicates(where_clause, &mut rng);
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            mutate_where_clause_predicates(where_clause, &mut rng);
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                if let Some(where_clause) = &mut struct_item.generics.where_clause {
                    mutate_where_clause_predicates(where_clause, &mut rng);
                }
            }
            if let syn::Item::Enum(enum_item) = item {
                if let Some(where_clause) = &mut enum_item.generics.where_clause {
                    mutate_where_clause_predicates(where_clause, &mut rng);
                }
            }
            if let syn::Item::Trait(trait_item) = item {
                if let Some(where_clause) = &mut trait_item.generics.where_clause {
                    mutate_where_clause_predicates(where_clause, &mut rng);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where clauses that constrain associated type projections with generic arguments. It swaps two type parameters within the angle brackets of the projection, reordering the arguments. This transformation tests the compiler's handling of associated type projections, generic argument ordering, and trait resolution under altered type parameter sequences. It aims to uncover bugs in type inference, projection validation, and trait bound satisfaction when argument order is permuted."
    }
}

fn mutate_where_clause_predicates(where_clause: &mut syn::WhereCl1ause, rng: &mut impl Rng) {
    for predicate in &mut where_clause.predicates {
        if let syn::WherePredicate::Type(predicate_type) = predicate {
            if let syn::Type::Path(type_path) = &mut predicate_type.bounded_ty {
                let last_segment = type_path.path.segments.last_mut();
                if let Some(last_segment) = last_segment {
                    if let syn::PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        let generic_args = &mut args.args;
                        if generic_args.len() >= 2 {
                            let indices: Vec<usize> = (0..generic_args.len()).collect();
                            let mut candidates = Vec::new();
                            for i in 0..generic_args.len() {
                                for j in i+1..generic_args.len() {
                                    if are_distinct_type_params(&generic_args[i], &generic_args[j]) {
                                        candidates.push((i, j));
                                    }
                                }
                            }
                            if !candidates.is_empty() {
                                let (i, j) = candidates.choose(rng).unwrap();
                                let args_vec = generic_args.clone();
                                generic_args.clear();
                                for (idx, arg) in args_vec.iter().enumerate() {
                                    if idx == *i {
                                        generic_args.push(args_vec[*j].clone());
                                    } else if idx == *j {
                                        generic_args.push(args_vec[*i].clone());
                                    } else {
                                        generic_args.push(arg.clone());
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

fn are_distinct_type_params(arg1: &syn::GenericArgument, arg2: &syn::GenericArgument) -> bool {
    match (arg1, arg2) {
        (syn::GenericArgument::Type(type1), syn::GenericArgument::Type(type2)) => {
            if let (syn::Type::Path(path1), syn::Type::Path(path2)) = (type1, type2) {
                path1.path.segments.len() == 1 && path2.path.segments.len() == 1 &&
                path1.path.segments[0].arguments == syn::PathArguments::None &&
                path2.path.segments[0].arguments == syn::PathArguments::None &&
                path1.path.segments[0].ident != path2.path.segments[0].ident
            } else {
                false
            }
        }
        _ => false,
    }
}