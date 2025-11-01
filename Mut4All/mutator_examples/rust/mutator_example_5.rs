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

pub struct Modify_WhereClause_With_Infer_Fn_Bound_5;

impl Mutator for Modify_WhereClause_With_Infer_Fn_Bound_5 {
    fn name(&self) -> &str {
        "Modify_WhereClause_With_Infer_Fn_Bound_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let fn_bound = TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path::from(Ident::new("Fn", Span::call_site())),
                };
                let mut bounds = Punctuated::new();
                bounds.push(syn::TypeParamBound::Trait(fn_bound));
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
                        let fn_bound = TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path::from(Ident::new("Fn", Span::call_site())),
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(syn::TypeParamBound::Trait(fn_bound));
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
        "The mutation operator injects a `where _ : Fn` clause into function and method signatures. By introducing an inferred type with a trait bound, it creates ambiguous or incomplete type constraints. This transformation aims to test the compiler's ability to resolve underspecified trait bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}
