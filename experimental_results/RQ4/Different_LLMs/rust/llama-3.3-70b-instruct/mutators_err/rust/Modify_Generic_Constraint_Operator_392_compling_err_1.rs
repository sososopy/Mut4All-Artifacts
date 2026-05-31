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

pub struct Modify_Generic_Constraint_Operator_392;

impl Mutator for Modify_Generic_Constraint_Operator_392 {
    fn name(&self) -> &str {
        "Modify_Generic_Constraint_Operator_392"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                                let path_segments = &type_path.path.segments;
                                if path_segments.len() > 0 {
                                    let last_segment = path_segments.last().unwrap();
                                    if last_segment.ident == "Iterator" {
                                        let new_bound = TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![
                                                    PathSegment {
                                                        ident: Ident::new("Clone", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    },
                                                ]),
                                            },
                                        });
                                        predicate_type.bounds.push(new_bound);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                                        let path_segments = &type_path.path.segments;
                                        if path_segments.len() > 0 {
                                            let last_segment = path_segments.last().unwrap();
                                            if last_segment.ident == "Iterator" {
                                                let new_bound = TypeParamBound::Trait(TraitBound {
                                                    paren_token: None,
                                                    modifier: TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![
                                                            PathSegment {
                                                                ident: Ident::new("Clone", Span::call_site()),
                                                                arguments: PathArguments::None,
                                                            },
                                                        ]),
                                                    },
                                                });
                                                predicate_type.bounds.push(new_bound);
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
        "The mutation operator targets the where clause of generic functions and type definitions, specifically the trait bounds on type parameters. It modifies the trait bounds by adding a new bound, in this case, the Clone trait, to test the compiler's handling of different constraint combinations and potential issues with trait resolution, coherence, or constraint solving."
    }
}