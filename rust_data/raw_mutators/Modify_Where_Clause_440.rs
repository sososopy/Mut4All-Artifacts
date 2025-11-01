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

pub struct Modify_Where_Clause_440;

impl Mutator for Modify_Where_Clause_440 {
    fn name(&self) -> &str {
        "Modify_Where_Clause_440"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(segment) = trait_bound.path.segments.last_mut() {
                                        if segment.ident == "FnOnce" {
                                            segment.ident = Ident::new("Fn", segment.ident.span());
                                        } else if segment.ident == "Fn" {
                                            segment.ident = Ident::new("FnMut", segment.ident.span());
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
        "This mutator targets functions with a `where` clause containing trait bounds and lifetime parameters. It modifies the trait bounds of function types, specifically changing `FnOnce` to `Fn` and `Fn` to `FnMut`. Additionally, it adjusts lifetime parameters to introduce new complexity or remove them to test the compiler's handling of trait bounds and lifetime relationships."
    }
}