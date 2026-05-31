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

pub struct Add_Unused_Lifetime_Constraints_146;

impl Mutator for Add_Unused_Lifetime_Constraints_146 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Constraints_146"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut new_lifetime = Lifetime::new("'unused", Span::call_site());
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: new_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                        lifetime: new_lifetime.clone(),
                        colon_token: Default::default(),
                        bounds: Punctuated::new(),
                    }));
                } else {
                    func.sig.generics.where_clause = Some(syn::WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                lifetime: new_lifetime,
                                colon_token: Default::default(),
                                bounds: Punctuated::new(),
                            }));
                            predicates
                        },
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter and constraint to function signatures. By introducing lifetimes that are not used in the function body or parameters, it aims to test the compiler's handling of redundant lifetime constraints and may reveal bugs related to lifetime resolution, inference, or error reporting."
    }
}