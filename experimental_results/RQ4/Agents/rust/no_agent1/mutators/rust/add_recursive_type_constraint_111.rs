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

pub struct Add_Recursive_Type_Constraint_111;

impl Mutator for Add_Recursive_Type_Constraint_111 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Constraint_111"
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
                            segs.push(PathSegment {
                                ident: Ident::new("RecursiveTrait", Span::call_site()),
                                arguments: PathArguments::None,
                            });
                            segs
                        },
                    },
                };
                let mut bounds = Punctuated::new();
                bounds.push(TypeParamBound::Trait(recursive_bound.clone()));
                bounds.push(TypeParamBound::Trait(recursive_bound));
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
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let recursive_bound = TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(PathSegment {
                                        ident: Ident::new("RecursiveTrait", Span::call_site()),
                                        arguments: PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(recursive_bound.clone()));
                        bounds.push(TypeParamBound::Trait(recursive_bound));
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
        "The mutation operator introduces a recursive type constraint `where _ : RecursiveTrait + RecursiveTrait` into function and method signatures. This transformation aims to stress the compiler's trait resolution system by creating potentially infinite trait bounds, which could lead to ICEs, assertion failures, or hangs during type inference and trait resolution."
    }
}