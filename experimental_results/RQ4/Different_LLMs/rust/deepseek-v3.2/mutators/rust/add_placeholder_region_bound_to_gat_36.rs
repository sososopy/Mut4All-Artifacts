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

pub struct Add_Placeholder_Region_Bound_To_GAT_36;

impl Mutator for Add_Placeholder_Region_Bound_To_GAT_36 {
    fn name(&self) -> &str {
        "Add_Placeholder_Region_Bound_To_GAT_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut lifetime_counter = 0;
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = trait_item {
                        let generics = &mut type_item.generics;
                        if generics.params.iter().any(|p| matches!(p, GenericParam::Lifetime(_))) {
                            lifetime_counter += 1;
                            let placeholder_name = format!("'struct_span_err{}", lifetime_counter);
                            let placeholder_lifetime = Lifetime::new(&placeholder_name, Span::call_site());
                            let new_predicate = syn::WherePredicate::Type(syn::PredicateType {
                                lifetimes: None,
                                bounded_ty: parse_quote!(Self),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(syn::TypeParamBound::Lifetime(placeholder_lifetime));
                                    bounds
                                },
                            });
                            if let Some(where_clause) = &mut type_item.generics.where_clause {
                                where_clause.predicates.push(new_predicate);
                            } else {
                                let mut predicates = Punctuated::new();
                                predicates.push(new_predicate);
                                type_item.generics.where_clause = Some(syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates,
                                });
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        let generics = &mut type_item.generics;
                        if generics.params.iter().any(|p| matches!(p, GenericParam::Lifetime(_))) {
                            lifetime_counter += 1;
                            let placeholder_name = format!("'let_binding{}", lifetime_counter);
                            let placeholder_lifetime = Lifetime::new(&placeholder_name, Span::call_site());
                            let new_predicate = syn::WherePredicate::Type(syn::PredicateType {
                                lifetimes: None,
                                bounded_ty: parse_quote!(Self),
                                colon_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(syn::TypeParamBound::Lifetime(placeholder_lifetime));
                                    bounds
                                },
                            });
                            if let Some(where_clause) = &mut type_item.generics.where_clause {
                                where_clause.predicates.push(new_predicate);
                            } else {
                                let mut predicates = Punctuated::new();
                                predicates.push(new_predicate);
                                type_item.generics.where_clause = Some(syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates,
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic associated types (GATs) with lifetime parameters in trait declarations and implementations. It adds a new where clause predicate `Self: 'placeholder` using a freshly generated lifetime name, mimicking the pattern that triggered an ICE. This introduces bogus region bounds to stress borrow checker region inference and opaque type handling, potentially exposing compiler bugs."
    }
}