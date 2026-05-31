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

pub struct Modify_Lifetime_Annotations_39;

impl Mutator for Modify_Lifetime_Annotations_39 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                if let Some(bound_lifetimes) = func.sig.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        Some(lifetime_param)
                    } else {
                        None
                    }
                }) {
                    let new_lifetime_ident = Ident::new(
                        &format!("'{}", ('a' as u8 + rng.gen_range(1..26)) as char),
                        Span::call_site(),
                    );
                    bound_lifetimes.lifetime = Lifetime::new(&new_lifetime_ident.to_string(), Span::call_site());
                }
                
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                        for lifetime in &mut lifetimes.lifetimes {
                                            let new_lifetime_ident = Ident::new(
                                                &format!("'{}", ('a' as u8 + rng.gen_range(1..26)) as char),
                                                Span::call_site(),
                                            );
                                            lifetime.lifetime = Lifetime::new(&new_lifetime_ident.to_string(), Span::call_site());
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
        "The mutation operator modifies lifetime annotations in function signatures that use higher-ranked trait bounds. It changes existing lifetime parameters to a randomly chosen different lifetime. This transformation aims to test the compiler's handling of lifetimes and their interaction with complex type structures, potentially revealing issues in lifetime resolution and trait bound processing."
    }
}