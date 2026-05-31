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

pub struct Replace_Tuple_Arg_In_AsyncFnOnce_496;

impl Mutator for Replace_Tuple_Arg_In_AsyncFnOnce_496 {
    fn name(&self) -> &str {
        "Replace_Tuple_Arg_In_AsyncFnOnce_496"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred) = predicate {
                            for bound in &mut pred.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(segment) = trait_bound.path.segments.iter_mut().last() {
                                        if segment.ident == "AsyncFnOnce" {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                if args.args.len() == 1 {
                                                    if let syn::GenericArgument::Type(ty) = args.args.first_mut().unwrap() {
                                                        if let syn::Type::Tuple(tuple_ty) = &*ty {
                                                            if tuple_ty.elems.len() == 1 {
                                                                let inner_ty = tuple_ty.elems.first().unwrap().clone();
                                                                *ty = inner_ty;
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(pred) = predicate {
                                    for bound in &mut pred.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(segment) = trait_bound.path.segments.iter_mut().last() {
                                                if segment.ident == "AsyncFnOnce" {
                                                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                        if args.args.len() == 1 {
                                                            if let syn::GenericArgument::Type(ty) = args.args.first_mut().unwrap() {
                                                                if let syn::Type::Tuple(tuple_ty) = &*ty {
                                                                    if tuple_ty.elems.len() == 1 {
                                                                        let inner_ty = tuple_ty.elems.first().unwrap().clone();
                                                                        *ty = inner_ty;
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}