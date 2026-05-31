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

pub struct Replace_For_Lifetime_Constraint_24;

impl Mutator for Replace_For_Lifetime_Constraint_24 {
    fn name(&self) -> &str {
        "Replace_For_Lifetime_Constraint_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let Some(lifetime) = predicate_type.lifetimes.as_ref() {
                                if lifetime.lifetimes.len() == 1 {
                                    if let Some(lifetime_param) = lifetime.lifetimes.first() {
                                        if lifetime_param.lifetime.ident == "a" {
                                            predicate_type.bounds.clear();
                                            predicate_type.bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                                paren_token: None,
                                                modifier: syn::TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: {
                                                        let mut segs = Punctuated::new();
                                                        segs.push(syn::PathSegment {
                                                            ident: Ident::new("Trait", Span::call_site()),
                                                            arguments: syn::PathArguments::AngleBracketed(
                                                                syn::AngleBracketedGenericArguments {
                                                                    colon2_token: None,
                                                                    lt_token: token::Lt::default(),
                                                                    args: {
                                                                        let mut args = Punctuated::new();
                                                                        args.push(syn::GenericArgument::Lifetime(syn::Lifetime::new("'static", Span::call_site())));
                                                                        args.push(syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                                            qself: None,
                                                                            path: syn::Path::from(Ident::new("u32", Span::call_site())),
                                                                        })));
                                                                        args
                                                                    },
                                                                    gt_token: token::Gt::default(),
                                                                }
                                                            ),
                                                        });
                                                        segs
                                                    },
                                                },
                                            }));
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
        ""
    }
}