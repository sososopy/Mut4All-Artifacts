use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, WhereClause, WherePredicate,
};

use crate::mutator::Mutator;

pub struct Modify_Function_Generics_And_Traits_440;

impl Mutator for Modify_Function_Generics_And_Traits_440 {
    fn name(&self) -> &str {
        "Modify_Function_Generics_And_Traits_440"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                if let Some(where_clause) = &mut generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let new_trait_bound: TypeParamBound = parse_quote!(T: Fn(U));
                            predicate_type.bounds.push(new_trait_bound);
                        }
                    }
                } else {
                    let new_where_clause: WhereClause = parse_quote!(where T: Fn(U));
                    generics.where_clause = Some(new_where_clause);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function generics by adding a `T: Fn(U)` trait bound to existing where clauses or creates a new where clause if none exists. This transformation introduces non-lifetime binders, testing the compiler's handling of trait resolution and type inference in complex generic contexts."
    }
}