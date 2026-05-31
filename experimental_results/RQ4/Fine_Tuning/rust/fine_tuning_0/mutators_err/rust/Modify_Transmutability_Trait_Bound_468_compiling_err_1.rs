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

pub struct Modify_Transmutability_Trait_Bound_468;

impl Mutator for Modify_Transmutability_Trait_Bound_468 {
    fn name(&self) -> &str {
        "Modify_Transmutability_Trait_Bound_468"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "BikeshedIntrinsicFrom" {
                                        last_segment.ident = Ident::new("Assume", Span::call_site());
                                    }
                                }
                            }
                        }
                    }
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(syn::WherePredicate::Type(PredicateType {
                                lifetimes: None,
                                bounded_ty: syn::Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(syn::PathSegment {
                                                ident: Ident::new("Dst", Span::call_site()),
                                                arguments: syn::PathArguments::None,
                                            });
                                            segs
                                        },
                                    },
                                }),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(syn::TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segs = Punctuated::new();
                                                segs.push(syn::PathSegment {
                                                    ident: Ident::new("Assume", Span::call_site()),
                                                    arguments: syn::PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: syn::punctuated::Punctuated::from_iter(
                                                                vec![GenericArgument::Type(
                                                                    Type::Path(TypePath {
                                                                        qself: None,
                                                                        path: syn::Path::from(Ident::new(
                                                                            "Src",
                                                                            Span::call_site(),
                                                                        )),
                                                                    }),
                                                                )],
                                                            ),
                                                            gt_token: token::Gt::default(),
                                                        },
                                                    ),
                                                });
                                                segs
                                            },
                                        },
                                    }));
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}