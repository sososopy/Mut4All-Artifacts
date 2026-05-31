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
use syn::For;

use crate::mutator::Mutator;

pub struct Late_Bound_Const_Capture_Conflict_443;

impl Mutator for Late_Bound_Const_Capture_Conflict_443 {
    fn name(&self) -> &str {
        "Late_Bound_Const_Capture_Conflict_443"
    }
    fn mutate(&self, file: &mut syn::File) {
        if !file.attrs.iter().any(|attr| {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") && list.nested().iter().any(|n| {
                    if let syn::NestedMeta::Meta(syn::Meta::NameValue(name_val)) = n {
                        if name_val.path.is_ident("non_lifetime_binders") {
                            return true;
                        }
                    }
                    false
                }) {
                    return true;
                }
            }
            false
        }) {
            let feature_attr = parse_quote! {
                #![feature(non_lifetime_binders)]
            };
            file.attrs.insert(0, feature_attr);
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in predicate_type.bounds.iter() {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let generic_param = parse_quote!(const C: usize);
                                    let bound_lifetimes = BoundLifetimes {
                                        for_token: Token![for](Span::call_site()),
                                        lifetimes: Punctuated::from_iter(vec![generic_param]),
                                        lt_token: Token![<](Span::call_site()),
                                        gt_token: Token![>](Span::call_site()),
                                    };
                                    let bound_for = For {
                                        for_token: Token![for](Span::call_site()),
                                        bound_lifetimes,
                                        lt_token: Token![<](Span::call_site()),
                                        bound: Box::new(bound.clone()),
                                        gt_token: Token![>](Span::call_site()),
                                    };
                                    let for_bound = TypeParamBound::For(bound_for);
                                    new_bounds.push(for_bound);
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if let Some(where_clause) = &mut impl_item.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            let mut new_bounds = Punctuated::new();
                            for bound in predicate_type.bounds.iter() {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let generic_param = parse_quote!(const C: usize);
                                    let bound_lifetimes = BoundLifetimes {
                                        for_token: Token![for](Span::call_site()),
                                        lifetimes: Punctuated::from_iter(vec![generic_param]),
                                        lt_token: Token![<](Span::call_site()),
                                        gt_token: Token![>](Span::call_site()),
                                    };
                                    let bound_for = For {
                                        for_token: Token![for](Span::call_site()),
                                        bound_lifetimes,
                                        lt_token: Token![<](Span::call_site()),
                                        bound: Box::new(bound.clone()),
                                        gt_token: Token![>](Span::call_site()),
                                    };
                                    let for_bound = TypeParamBound::For(bound_for);
                                    new_bounds.push(for_bound);
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            predicate_type.bounds = new_bounds;
                        }
                    }
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in predicate_type.bounds.iter() {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            let generic_param = parse_quote!(const C: usize);
                                            let bound_lifetimes = BoundLifetimes {
                                                for_token: Token![for](Span::call_site()),
                                                lifetimes: Punctuated::from_iter(vec![generic_param]),
                                                lt_token: Token![<](Span::call_site()),
                                                gt_token: Token![>](Span::call_site()),
                                            };
                                            let bound_for = For {
                                                for_token: Token![for](Span::call_site()),
                                                bound_lifetimes,
                                                lt_token: Token![<](Span::call_site()),
                                                bound: Box::new(bound.clone()),
                                                gt_token: Token![>](Span::call_site()),
                                            };
                                            let for_bound = TypeParamBound::For(bound_for);
                                            new_bounds.push(for_bound);
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    }
                                    predicate_type.bounds = new_bounds;
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