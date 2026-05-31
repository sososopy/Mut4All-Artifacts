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

pub struct Replace_Generic_Const_Exprs_With_Constants_378;

impl Mutator for Replace_Generic_Const_Exprs_With_Constants_378 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Exprs_With_Constants_378"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                        for arg in &mut angle_bracketed.args {
                                            if let GenericArgument::Const(const_arg) = arg {
                                                let new_expr: Expr = parse_quote! { 0 };
                                                *const_arg = syn::GenericArgument::Const(syn::Const {
                                                    span: const_arg.span(),
                                                    token: token::Const { span: const_arg.span() },
                                                    expr: Box::new(new_expr),
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                                        for segment in &mut type_path.path.segments {
                                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                                for arg in &mut angle_bracketed.args {
                                                    if let GenericArgument::Const(const_arg) = arg {
                                                        let new_expr: Expr = parse_quote! { 0 };
                                                        *const_arg = syn::GenericArgument::Const(syn::Const {
                                                            span: const_arg.span(),
                                                            token: token::Const { span: const_arg.span() },
                                                            expr: Box::new(new_expr),
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic const expressions with simple constants to test the compiler's ability to handle the absence of complex const expressions."
    }
}