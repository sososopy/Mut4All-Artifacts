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

pub struct Function_Signature_With_Lifetime_Constraints_274;

impl Mutator for Function_Signature_With_Lifetime_Constraints_274 {
    fn name(&self) -> &str {
        "Function_Signature_With_Lifetime_Constraints_274"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                                        let mut new_bound_lifetimes = bound_lifetimes.clone();
                                        if !new_bound_lifetimes.lifetimes.is_empty() {
                                            let mut rng = thread_rng();
                                            let index = rng
                                                .gen_range(0..new_bound_lifetimes.lifetimes.len());
                                            let chosen_lifetime =
                                                new_bound_lifetimes.lifetimes[index].clone();
                                            let new_lifetime_name =
                                                format!("b{}", chosen_lifetime.lifetime.ident);
                                            let new_lifetime = Lifetime::new(
                                                &new_lifetime_name,
                                                Span::call_site(),
                                            );
                                            new_bound_lifetimes.lifetimes[index] =
                                                LifetimeParam::new(new_lifetime);
                                            trait_bound.lifetimes = Some(new_bound_lifetimes);
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
        "This mutator targets functions with generic parameters that have lifetime constraints in their where clauses. It modifies the lifetime parameters within trait bounds by introducing a new lifetime name based on the existing one. This transformation aims to stress-test the compiler's handling of lifetime resolution in trait bounds, potentially exposing bugs related to lifetime inference and trait system interactions."
    }
}