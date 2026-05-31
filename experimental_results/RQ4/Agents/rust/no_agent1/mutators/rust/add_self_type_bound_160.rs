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

pub struct Add_Self_Type_Bound_160;

impl Mutator for Add_Self_Type_Bound_160 {
    fn name(&self) -> &str {
        "Add_Self_Type_Bound_160"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let self_bound = TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("Self", Span::call_site()),
                                arguments: syn::PathArguments::None,
                            });
                            segs.push(syn::PathSegment {
                                ident: Ident::new("Sized", Span::call_site()),
                                arguments: syn::PathArguments::None,
                            });
                            segs
                        },
                    },
                };
                let mut bounds = Punctuated::new();
                bounds.push(syn::TypeParamBound::Trait(self_bound));
                func.sig.generics.where_clause = Some(WhereClause {
                    where_token: Default::default(),
                    predicates: {
                        let mut predicates = Punctuated::new();
                        predicates.push(syn::WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: syn::Type::Infer(syn::TypeInfer {
                                underscore_token: Default::default(),
                            }),
                            colon_token: Default::default(),
                            bounds,
                        }));
                        predicates
                    },
                });
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let self_bound = TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new("Self", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new("Sized", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(syn::TypeParamBound::Trait(self_bound));
                        func.sig.generics.where_clause = Some(WhereClause {
                            where_token: Default::default(),
                            predicates: {
                                let mut predicates = Punctuated::new();
                                predicates.push(syn::WherePredicate::Type(PredicateType {
                                    lifetimes: None,
                                    bounded_ty: syn::Type::Infer(syn::TypeInfer {
                                        underscore_token: Default::default(),
                                    }),
                                    colon_token: Default::default(),
                                    bounds,
                                }));
                                predicates
                            },
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a `where _ : Self + Sized` clause to function and method signatures. By introducing a self-referential trait bound, it creates complex type constraints that may lead to ICEs or resolution failures. This transformation tests the compiler's handling of self-referential and sized trait bounds, potentially exposing bugs in type inference and trait resolution."
    }
}