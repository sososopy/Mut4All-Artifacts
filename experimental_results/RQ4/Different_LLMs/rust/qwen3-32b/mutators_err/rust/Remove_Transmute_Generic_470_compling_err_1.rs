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

pub struct Remove_Transmute_Generic_470;

impl Mutator for Remove_Transmute_Generic_470 {
    fn name(&self) -> &str {
        "Remove_Transmute_Generic_470"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                // Check if there's a TransmuteFrom constraint
                let has_transmute = {
                    let mut has = false;
                    if let Some(where_clause) = &generics.where_clause {
                        for predicate in &where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                for bound in &predicate_type.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("TransmuteFrom") {
                                            has = true;
                                            break;
                                        }
                                    }
                                }
                                if has { break; }
                            }
                        }
                    }
                    has
                };
                if !has_transmute {
                    // Add dummy TransmuteFrom constraint
                    // Check if there's at least one type parameter
                    let has_type_params = generics.params.iter().any(|param| matches!(param, GenericParam::Type(_)));
                    if !has_type_params {
                        generics.params.push(parse_quote!(Dst));
                    }
                    // Add Src type parameter
                    generics.params.push(parse_quote!(Src));
                    // Add where clause
                    let where_clause = generics.where_clause.get_or_insert_with(|| syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Default::default(),
                    });
                    // Choose Dst as the type to apply the constraint
                    let dst_type = if let Some(param) = generics.params.iter().find(|param| matches!(param, GenericParam::Type(type_param) if type_param.ident == "Dst")) {
                        parse_quote!(Dst)
                    } else {
                        // Use the first type parameter
                        if let Some(param) = generics.params.iter().find(|param| matches!(param, GenericParam::Type(_))) {
                            if let GenericParam::Type(type_param) = param {
                                parse_quote!(#type_param)
                            } else {
                                unreachable!()
                            }
                        } else {
                            unreachable!()
                        }
                    };
                    let src_type = parse_quote!(Src);
                    let transmute_bound = parse_quote!(TransmuteFrom<#src_type>);
                    let predicate = syn::WherePredicate::Type(syn::PredicateType {
                        lifetimes: None,
                        bounded_ty: Box::new(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: dst_type.path,
                        })),
                        colon_token: Default::default(),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: transmute_bound.path,
                            }));
                            bounds
                        },
                    });
                    where_clause.predicates.push(predicate);
                }
                // Now, find and remove the Src type parameter
                let type_params: HashSet<Ident> = generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if let Some(where_clause) = &generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("TransmuteFrom") {
                                        if let syn::PathArguments::AngleBracketed(args) = &trait_bound.path.segments[0].arguments {
                                            if let Some(first_arg) = args.args.iter().next() {
                                                if let syn::GenericArgument::Type(src_type) = first_arg {
                                                    let src_ident = if let Type::Path(type_path) = &**src_type {
                                                        if let Some(segment) = type_path.path.segments.iter().next() {
                                                            Some(segment.ident.clone())
                                                        } else {
                                                            None
                                                        }
                                                    } else {
                                                        None
                                                    };
                                                    if let Some(src_ident) = src_ident {
                                                        if type_params.contains(&src_ident) {
                                                            // Remove the src type parameter from generics
                                                            let mut to_remove = None;
                                                            for (i, param) in generics.params.iter().enumerate() {
                                                                if let GenericParam::Type(type_param) = param {
                                                                    if type_param.ident == src_ident {
                                                                        to_remove = Some(i);
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                            if let Some(index) = to_remove {
                                                                generics.params.remove(index);
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
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with a `TransmuteFrom<Src, Dst>`-like constraint and removes the `Src` type parameter from their generics, leaving it undefined in the where clause. This introduces an invalid reference to an undefined type, potentially triggering ICEs during size/layout calculations. If no such constraint exists, a dummy one is synthesized using existing or newly added type parameters."
    }
}