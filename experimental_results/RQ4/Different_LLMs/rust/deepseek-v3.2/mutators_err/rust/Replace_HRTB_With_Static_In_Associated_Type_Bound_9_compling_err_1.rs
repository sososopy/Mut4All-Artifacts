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

pub struct Replace_HRTB_With_Static_In_Associated_Type_Bound_9;

impl Mutator for Replace_HRTB_With_Static_In_Associated_Type_Bound_9 {
    fn name(&self) -> &str {
        "Replace_HRTB_With_Static_In_Associated_Type_Bound_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                mutate_where_clause(&mut func.sig.generics);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        mutate_where_clause(&mut func.sig.generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets higher-rank trait bounds (HRTBs) in where clauses that constrain associated type projections. It replaces the universal quantification 'for<'a>' with a static lifetime 'static' in the associated type projection. Specifically, it transforms bounds like 'where for<'a> <Foo<'a> as Bar>::Type: TraitBound' into 'where <Foo<'static> as Bar>::Type: TraitBound'. This mutation changes the meaning from requiring the bound for all lifetimes to requiring it only for the static lifetime, potentially exposing bugs in compiler handling of associated type projections and lifetime substitution in bound checking."
    }
}

fn mutate_where_clause(generics: &mut syn::Generics) {
    if let Some(where_clause) = &mut generics.where_clause {
        for predicate in &mut where_clause.predicates {
            if let syn::WherePredicate::Type(pred_type) = predicate {
                if let Some(bound_lifetimes) = &pred_type.lifetimes {
                    if bound_lifetimes.lifetimes.len() == 1 {
                        let lifetime_param = &bound_lifetimes.lifetimes[0];
                        let lifetime_name = lifetime_param.ident.to_string();
                        if let syn::Type::Path(type_path) = &pred_type.bounded_ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                                    for arg in &mut args.args {
                                        if let syn::GenericArgument::Lifetime(lifetime) = arg {
                                            if lifetime.ident.to_string() == lifetime_name {
                                                *lifetime = syn::Lifetime::new("'static", lifetime.span());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        pred_type.lifetimes = None;
                    }
                }
            }
        }
    }
}