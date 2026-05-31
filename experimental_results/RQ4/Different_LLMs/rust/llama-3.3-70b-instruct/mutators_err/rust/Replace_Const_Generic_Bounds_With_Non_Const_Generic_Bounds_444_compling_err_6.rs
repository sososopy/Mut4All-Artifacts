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

pub struct Replace_Const_Generic_Bounds_With_Non_Const_Generic_Bounds_444;

impl Mutator for Replace_Const_Generic_Bounds_With_Non_Const_Generic_Bounds_444 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bounds_With_Non_Const_Generic_Bounds_444"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(const_token) = trait_bound.path.segments.first() {
                                        if const_token.ident == "const" {
                                            let mut new_segments = trait_bound.path.segments.clone();
                                            new_segments.remove(0);
                                            trait_bound.path.segments = new_segments;
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
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    for bound in &mut predicate_type.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(const_token) = trait_bound.path.segments.first() {
                                                if const_token.ident == "const" {
                                                    let mut new_segments = trait_bound.path.segments.clone();
                                                    new_segments.remove(0);
                                                    trait_bound.path.segments = new_segments;
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
        "The mutation operator replaces const generic bounds with non-const generic bounds in function and method signatures. This transformation aims to test the compiler's handling of generic bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}