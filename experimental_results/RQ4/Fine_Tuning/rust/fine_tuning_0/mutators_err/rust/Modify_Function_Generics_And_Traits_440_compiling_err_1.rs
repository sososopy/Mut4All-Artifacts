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
                            let new_trait_bound: TypeParamBound = parse_quote!(for<U> T: Fn(U));
                            predicate_type.bounds.push(new_trait_bound);
                        }
                    }
                } else {
                    let new_where_clause: WhereClause = parse_quote!(where for<U> T: Fn(U));
                    generics.where_clause = Some(new_where_clause);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function generics by adding a `for<U> T: Fn(U)` trait bound to existing where clauses or creates a new where clause if none exists. This transformation introduces non-lifetime binders, testing the compiler's handling of trait resolution and type inference in complex generic contexts."
    }
}