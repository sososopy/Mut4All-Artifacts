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

pub struct Inject_Unused_Lifetime_Bounds_81;

impl Mutator for Inject_Unused_Lifetime_Bounds_81 {
    fn name(&self) -> &str {
        "Inject_Unused_Lifetime_Bounds_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(syn::WherePredicate::Lifetime(PredicateLifetime {
                        lifetime: Lifetime::new("'unused", Span::call_site()),
                        colon_token: Default::default(),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(Lifetime::new("'static", Span::call_site()));
                            bounds
                        },
                    }));
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Lifetime(PredicateLifetime {
                                lifetime: Lifetime::new("'unused", Span::call_site()),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(Lifetime::new("'static", Span::call_site()));
                                    bounds
                                },
                            }));
                            predicates
                        },
                    });
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetimes = &mut func.sig.generics.params;
                        lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            where_clause.predicates.push(syn::WherePredicate::Lifetime(PredicateLifetime {
                                lifetime: Lifetime::new("'unused", Span::call_site()),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(Lifetime::new("'static", Span::call_site()));
                                    bounds
                                },
                            }));
                        } else {
                            func.sig.generics.where_clause = Some(WhereClause {
                                where_token: Default::default(),
                                predicates: {
                                    let mut predicates = Punctuated::new();
                                    predicates.push(syn::WherePredicate::Lifetime(PredicateLifetime {
                                        lifetime: Lifetime::new("'unused", Span::call_site()),
                                        colon_token: Default::default(),
                                        bounds: {
                                            let mut bounds = Punctuated::new();
                                            bounds.push(Lifetime::new("'static", Span::call_site()));
                                            bounds
                                        },
                                    }));
                                    predicates
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces unused lifetime parameters and bounds into function and method signatures. By adding lifetimes that are not referenced in the function body or parameters, it creates potential for lifetime resolution conflicts and ICEs in the compiler's borrow checker and lifetime inference systems."
    }
}