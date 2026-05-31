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

pub struct Add_Higher_Ranked_Trait_Bound_With_Future_494;

impl Mutator for Add_Higher_Ranked_Trait_Bound_With_Future_494 {
    fn name(&self) -> &str {
        "Add_Higher_Ranked_Trait_Bound_With_Future_494"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for param in &mut impl_item.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        let mut new_bounds = Punctuated::new();
                        for bound in type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    let new_trait_bound = TraitBound {
                                        modifier: TraitBoundModifier::ForAll(BoundLifetimes {
                                            lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                                lifetime: Lifetime::new("'a", Span::call_site()),
                                            }]),
                                        }),
                                        ..trait_bound.clone()
                                    };
                                    new_bounds.push(syn::TypeParamBound::Trait(new_trait_bound));
                                } else {
                                    new_bounds.push(bound);
                                }
                            } else {
                                new_bounds.push(bound);
                            }
                        }
                        type_param.bounds = new_bounds;
                    }
                }

                let mut where_clause = impl_item.generics.where_clause.take().unwrap_or_else(|| {
                    syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    }
                });

                let predicate = parse_quote! {
                    for<'a> <T as FnOnce<(&'a (),)>>::Output: Future
                };

                where_clause.predicates.push(predicate);
                impl_item.generics.where_clause = Some(where_clause);
            } else if let syn::Item::Trait(trait_item) = item {
                for param in &mut trait_item.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        let mut new_bounds = Punctuated::new();
                        for bound in type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    let new_trait_bound = TraitBound {
                                        modifier: TraitBoundModifier::ForAll(BoundLifetimes {
                                            lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                                lifetime: Lifetime::new("'a", Span::call_site()),
                                            }]),
                                        }),
                                        ..trait_bound.clone()
                                    };
                                    new_bounds.push(syn::TypeParamBound::Trait(new_trait_bound));
                                } else {
                                    new_bounds.push(bound);
                                }
                            } else {
                                new_bounds.push(bound);
                            }
                        }
                        type_param.bounds = new_bounds;
                    }
                }

                let mut where_clause = trait_item.generics.where_clause.take().unwrap_or_else(|| {
                    syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    }
                });

                let predicate = parse_quote! {
                    for<'a> <T as FnOnce<(&'a (),)>>::Output: Future
                };

                where_clause.predicates.push(predicate);
                trait_item.generics.where_clause = Some(where_clause);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}