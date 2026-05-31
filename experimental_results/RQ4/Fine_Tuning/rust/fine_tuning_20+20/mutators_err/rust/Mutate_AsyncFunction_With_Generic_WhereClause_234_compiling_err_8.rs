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

pub struct Mutate_AsyncFunction_With_Generic_WhereClause_234;

impl Mutator for Mutate_AsyncFunction_With_Generic_WhereClause_234 {
    fn name(&self) -> &str {
        "Mutate_AsyncFunction_With_Generic_WhereClause_234"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() && !func.sig.generics.params.is_empty() {
                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        let mut found_for_lifetime = false;
                        for predicate in where_clause.predicates.iter() {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                for bound in &predicate_type.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                                            if bound_lifetimes
                                                .lifetimes
                                                .iter()
                                                .any(|lt| lt.colon_token.is_some())
                                            {
                                                found_for_lifetime = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if found_for_lifetime {
                            where_clause
                                .predicates
                                .push(parse_quote!(for<'a> ...: 'a,));
                        }
                    } else {
                        func.sig.generics.where_clause = Some(parse_quote!(
                            where for<'a> ...: 'a,
                        ));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions with generic parameters by adding a `for<'a> ...: 'a,` constraint to the `where` clause. It checks for existing `for` lifetimes in the `where` clause and appends the variadic constraint if found. If no `where` clause is present, it introduces one with the new constraint. This transformation leverages a complex syntax to stress-test the compiler's handling of lifetime and variadic constraints in async generic contexts."
    }
}