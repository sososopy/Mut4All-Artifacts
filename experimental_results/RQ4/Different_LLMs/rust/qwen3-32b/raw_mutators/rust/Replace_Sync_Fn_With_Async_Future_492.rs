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

pub struct Replace_Sync_Fn_With_Async_Future_492;

impl Mutator for Replace_Sync_Fn_With_Async_Future_492 {
    fn name(&self) -> &str {
        "Replace_Sync_Fn_With_Async_Future_492"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                // Add async keyword to the function signature
                func.sig.asyncness = Some(syn::Async::default());

                // Process the where clause
                let mut where_clause = func.sig.generics.where_clause.take().unwrap_or_else(|| {
                    syn::WhereClause {
                        where_token: syn::token::Where::default(),
                        predicates: Punctuated::new(),
                    }
                });

                let mut new_predicates = Punctuated::new();

                for predicate in where_clause.predicates {
                    if let syn::WherePredicate::Type(predicate_type) = predicate {
                        let mut new_bounds = Punctuated::new();

                        for bound in predicate_type.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    // Process this trait bound
                                    let new_trait_bound = process_fn_trait_bound(trait_bound);
                                    new_bounds.push(syn::TypeParamBound::Trait(new_trait_bound));
                                } else {
                                    // Keep other bounds
                                    new_bounds.push(bound);
                                }
                            } else {
                                // Keep other bounds
                                new_bounds.push(bound);
                            }
                        }

                        // Add Send and Sync as new bounds
                        new_bounds.push(parse_quote!(Send));
                        new_bounds.push(parse_quote!(Sync));
                        new_bounds.push(parse_quote!('a));

                        // Create a new PredicateType with the new bounds
                        let new_predicate_type = syn::PredicateType {
                            lifetimes: predicate_type.lifetimes,
                            bounded_ty: predicate_type.bounded_ty,
                            colon_token: predicate_type.colon_token,
                            bounds: new_bounds,
                        };

                        new_predicates.push(syn::WherePredicate::Type(new_predicate_type));
                    } else {
                        // Keep other predicates
                        new_predicates.push(predicate);
                    }
                }

                where_clause.predicates = new_predicates;
                func.sig.generics.where_clause = Some(where_clause);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn process_fn_trait_bound(trait_bound: TraitBound) -> TraitBound {
    // Create the for<'a> modifier
    let modifier = TraitBoundModifier::For;

    // Create the bound lifetimes with 'a
    let lifetimes = BoundLifetimes {
        lifetimes: Punctuated::from_iter(vec![LifetimeParam {
            lifetime: Lifetime::new("'a", Span::call_site()),
        }]),
    };

    // Modify the Fn's generic arguments to include Future<Output=...>
    let new_path = modify_fn_path_arguments(trait_bound.path);

    TraitBound {
        modifier,
        lifetimes: Some(lifetimes),
        path: new_path,
        paren_token: trait_bound.paren_token,
    }
}

fn modify_fn_path_arguments(mut path: SynPath) -> SynPath {
    if let PathArguments::AngleBracketed(args) = &mut path.arguments {
        let mut new_args = Punctuated::new();

        for arg in &args.args {
            if let GenericArgument::Binding(binding) = arg {
                if binding.ident == "Output" {
                    // Replace the Output binding with Future<Output = ...>
                    let output_type = &binding.value;

                    let future_type = parse_quote! {
                        std::future::Future<Output = #output_type>
                    };

                    new_args.push(GenericArgument::Binding(syn::GenericArgument::Binding {
                        ident: binding.ident.clone(),
                        eq_token: binding.eq_token.clone(),
                        value: Box::new(future_type),
                    }));
                } else {
                    // Keep other bindings
                    new_args.push(arg.clone());
                }
            } else {
                // Keep other arguments
                new_args.push(arg.clone());
            }
        }

        args.args = new_args;
    }

    path
}