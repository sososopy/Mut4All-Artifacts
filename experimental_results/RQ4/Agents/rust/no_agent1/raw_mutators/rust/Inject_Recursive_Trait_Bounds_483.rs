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

pub struct Inject_Recursive_Trait_Bounds_483;

impl Mutator for Inject_Recursive_Trait_Bounds_483 {
    fn name(&self) -> &str {
        "Inject_Recursive_Trait_Bounds_483"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_bound = TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("RecursiveTrait", Span::call_site()),
                                arguments: syn::PathArguments::None,
                            });
                            segs
                        },
                    },
                };
                let mut bounds = Punctuated::new();
                bounds.push(syn::TypeParamBound::Trait(recursive_bound.clone()));
                bounds.push(syn::TypeParamBound::Trait(recursive_bound));
                func.sig.generics.params.push(GenericParam::Type(parse_quote!(T: RecursiveTrait)));
                func.sig.generics.where_clause = Some(WhereClause {
                    where_token: Default::default(),
                    predicates: {
                        let mut predicates = Punctuated::new();
                        predicates.push(syn::WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: syn::Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("T", Span::call_site())),
                            }),
                            colon_token: Default::default(),
                            bounds,
                        }));
                        predicates
                    },
                });
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let recursive_bound = TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new("RecursiveTrait", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(syn::TypeParamBound::Trait(recursive_bound.clone()));
                        bounds.push(syn::TypeParamBound::Trait(recursive_bound));
                        func.sig.generics.params.push(GenericParam::Type(parse_quote!(T: RecursiveTrait)));
                        func.sig.generics.where_clause = Some(WhereClause {
                            where_token: Default::default(),
                            predicates: {
                                let mut predicates = Punctuated::new();
                                predicates.push(syn::WherePredicate::Type(PredicateType {
                                    lifetimes: None,
                                    bounded_ty: syn::Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(Ident::new("T", Span::call_site())),
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
        "The mutation operator introduces recursive trait bounds into function and method signatures. By adding multiple identical trait bounds, it creates potentially infinite trait resolution paths, challenging the compiler's ability to handle recursive trait constraints and potentially leading to ICEs or hangs in the trait resolution system."
    }
}