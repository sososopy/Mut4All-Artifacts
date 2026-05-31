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

pub struct Inject_Unused_Lifetime_Bounds_20;

impl Mutator for Inject_Unused_Lifetime_Bounds_20 {
    fn name(&self) -> &str {
        "Inject_Unused_Lifetime_Bounds_20"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));

                if let Some(where_clause) = &mut generics.where_clause {
                    where_clause.predicates.push(syn::WherePredicate::Lifetime(
                        syn::PredicateLifetime {
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: Default::default(),
                            bounds: Punctuated::new(),
                        },
                    ));
                } else {
                    generics.where_clause = Some(syn::WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Lifetime(syn::PredicateLifetime {
                                lifetime: Lifetime::new("'unused", Span::call_site()),
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
        "The mutation operator introduces unused lifetime parameters and bounds into function signatures. This transformation aims to stress the compiler's lifetime resolution system, potentially leading to ICEs or assertion failures due to the introduction of redundant and unutilized lifetime constraints."
    }
}