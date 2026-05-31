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

pub struct Modify_Trait_Lifetime_Constraint_30;

impl Mutator for Modify_Trait_Lifetime_Constraint_30 {
    fn name(&self) -> &str {
        "Modify_Trait_Lifetime_Constraint_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if let Some(where_clause) = &mut type_item.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                        if type_path.path.segments.iter().any(|seg| seg.ident == "Self") {
                                            predicate_type.bounds = parse_quote!(Self: 'b);
                                        }
                                    }
                                }
                            }
                        }
                        type_item.generics.params = parse_quote!(<'b>);
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.iter().any(|seg| seg.ident == "Trait") {
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Type(type_item) = impl_item {
                                if type_item.ident == "Ty" {
                                    type_item.generics.params = parse_quote!(<'c>);
                                    if let Some(where_clause) = &mut type_item.generics.where_clause {
                                        for predicate in &mut where_clause.predicates {
                                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                                if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                                    if type_path.path.segments.iter().any(|seg| seg.ident == "Self") {
                                                        predicate_type.bounds = parse_quote!(Self: 'c);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}