use proc_macro2::Span;
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, WhereClause, WherePredicate,
};

use crate::mutator::Mutator;

pub struct Introduce_Trivial_Bounds_In_Function_Signatures_6;

impl Mutator for Introduce_Trivial_Bounds_In_Function_Signatures_6 {
    fn name(&self) -> &str {
        "Introduce_Trivial_Bounds_In_Function_Signatures_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                let mut trivial_bounds = Punctuated::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let bound: WherePredicate = if type_param.ident == "str" {
                            parse_quote!(#type_param: Sized)
                        } else {
                            parse_quote!(#type_param: Copy)
                        };
                        trivial_bounds.push(bound);
                    }
                }

                if !trivial_bounds.is_empty() {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: trivial_bounds,
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces trivial bounds to function signatures' where clauses. By adding constraints like `T: Sized` or `T: Copy`, it attempts to expose issues in the compiler's handling of type bounds and constant evaluation. This can lead to unexpected behavior or ICEs when the compiler's assumptions about these bounds are challenged."
    }
}