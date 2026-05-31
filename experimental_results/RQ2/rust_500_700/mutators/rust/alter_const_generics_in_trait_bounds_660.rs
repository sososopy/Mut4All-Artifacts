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

pub struct Alter_Const_Generics_In_Trait_Bounds_660;

impl Mutator for Alter_Const_Generics_In_Trait_Bounds_660 {
    fn name(&self) -> &str {
        "Alter_Const_Generics_In_Trait_Bounds_660"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for segment in &path.segments {
                        if let syn::PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                            for arg in &angle_bracketed.args {
                                if let syn::GenericArgument::Const(expr) = arg {
                                    if let syn::Expr::Lit(syn::ExprLit { lit: syn::Lit::Int(lit_int), .. }) = expr {
                                        if lit_int.base10_parse::<usize>().unwrap_or(0) == 0 {
                                            if let Some(where_clause) = &mut item_impl.generics.where_clause {
                                                for predicate in &mut where_clause.predicates {
                                                    if let syn::WherePredicate::Type(predicate_type) = predicate {
                                                        if let syn::Type::Array(type_array) = &mut predicate_type.bounded_ty {
                                                            if let syn::Expr::Path(expr_path) = &type_array.len {
                                                                if expr_path.path.segments.len() > 1 {
                                                                    type_array.len = syn::Expr::Lit(syn::ExprLit {
                                                                        attrs: Vec::new(),
                                                                        lit: syn::Lit::Int(syn::LitInt::new("4", Span::call_site())),
                                                                    });
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