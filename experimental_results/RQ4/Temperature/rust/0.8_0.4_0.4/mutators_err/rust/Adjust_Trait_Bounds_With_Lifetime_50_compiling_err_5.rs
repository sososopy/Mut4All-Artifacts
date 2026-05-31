use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, token::Comma, token::Plus,
    visit::Visit, visit_mut::VisitMut, *,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath,
};

use crate::mutator::Mutator;

pub struct Adjust_Trait_Bounds_With_Lifetime_50;

impl Mutator for Adjust_Trait_Bounds_With_Lifetime_50 {
    fn name(&self) -> &str {
        "Adjust_Trait_Bounds_With_Lifetime_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(ref mut lifetimes) = trait_bound.lifetimes {
                                        let new_lifetime: LifetimeDef = parse_quote!('b);
                                        lifetimes.lifetimes.push(new_lifetime.clone());
                                        let new_trait_bound: TraitBound = parse_quote!(for<'b> Trait<'b>);
                                        predicate_type.bounds.push(syn::TypeParamBound::Trait(new_trait_bound));
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
        "This mutator targets function definitions with where clauses specifying trait bounds that include lifetime parameters. It introduces a new lifetime parameter and applies it to an additional trait bound. This transformation tests the compiler's handling of trait bounds with multiple lifetime parameters, potentially exposing issues related to type inference, lifetime resolution, or trait implementation."
    }
}