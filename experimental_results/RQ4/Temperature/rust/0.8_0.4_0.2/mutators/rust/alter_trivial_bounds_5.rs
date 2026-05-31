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

pub struct Alter_Trivial_Bounds_5;

impl Mutator for Alter_Trivial_Bounds_5 {
    fn name(&self) -> &str {
        "Alter_Trivial_Bounds_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.path.is_ident("str") {
                                    predicate_type.bounded_ty = syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(Ident::new("i32", Span::call_site())),
                                    });
                                    predicate_type.bounds.clear();
                                    predicate_type.bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segments = Punctuated::new();
                                                segments.push(syn::PathSegment {
                                                    ident: Ident::new("Copy", Span::call_site()),
                                                    arguments: syn::PathArguments::None,
                                                });
                                                segments
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}