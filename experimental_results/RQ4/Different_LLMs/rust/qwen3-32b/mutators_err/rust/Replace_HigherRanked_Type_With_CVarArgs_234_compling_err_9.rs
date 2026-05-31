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

pub struct Replace_HigherRanked_Type_With_CVarArgs_234;

impl Mutator for Replace_HigherRanked_Type_With_CVarArgs_234 {
    fn name(&self) -> &str {
        "Replace_HigherRanked_Type_With_CVarArgs_234"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if let TraitBoundModifier::For = trait_bound.modifier {
                                    if let Some(last_segment) = trait_bound.path.segments.iter_mut().last() {
                                        if let syn::PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                            if let Some(arg) = angle_args.args.iter_mut().next() {
                                                if let syn::GenericArgument::Type(ty) = arg {
                                                    *ty = parse_quote!(...);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            for bound in &mut type_pred.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let TraitBoundModifier::For = trait_bound.modifier {
                                        if let Some(last_segment) = trait_bound.path.segments.iter_mut().last() {
                                            if let syn::PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                                if let Some(arg) = angle_args.args.iter_mut().next() {
                                                    if let syn::GenericArgument::Type(ty) = arg {
                                                        *ty = parse_quote!(...);
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for param in &mut method.sig.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if let TraitBoundModifier::For = trait_bound.modifier {
                                            if let Some(last_segment) = trait_bound.path.segments.iter_mut().last() {
                                                if let syn::PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                                    if let Some(arg) = angle_args.args.iter_mut().next() {
                                                        if let syn::GenericArgument::Type(ty) = arg {
                                                            *ty = parse_quote!(...);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(type_pred) = predicate {
                                    for bound in &mut type_pred.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            if let TraitBoundModifier::For = trait_bound.modifier {
                                                if let Some(last_segment) = trait_bound.path.segments.iter_mut().last() {
                                                    if let syn::PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                                        if let Some(arg) = angle_args.args.iter_mut().next() {
                                                            if let syn::GenericArgument::Type(ty) = arg {
                                                                *ty = parse_quote!(...);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}