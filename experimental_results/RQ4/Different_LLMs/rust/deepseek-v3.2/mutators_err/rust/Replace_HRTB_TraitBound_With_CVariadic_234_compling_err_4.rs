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

pub struct Replace_HRTB_TraitBound_With_CVariadic_234;

impl Mutator for Replace_HRTB_TraitBound_With_CVariadic_234 {
    fn name(&self) -> &str {
        "Replace_HRTB_TraitBound_With_CVariadic_234"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceHRTBVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets higher-ranked trait bounds (HRTBs) in where clauses, specifically patterns like `for<'a> T: Trait<...> + 'a`. It replaces the trait bound type (or the entire bound if it's a simple type) with a C-variadic placeholder `...`. This transformation corrupts the type specification while preserving the HRTB structure, potentially causing type resolution failures, lifetime mismatches, or ICEs in the compiler's handling of variadic generics and higher-ranked bounds."
    }
}

struct ReplaceHRTBVisitor;

impl VisitMut for ReplaceHRTBVisitor {
    fn visit_where_predicate_mut(&mut self, pred: &mut syn::WherePredicate) {
        if let syn::WherePredicate::Type(syn::PredicateType {
            lifetimes: Some(syn::BoundLifetimes {
                lifetimes: lt_params,
                for_token: _,
                ..
            }),
            bounded_ty: _,
            colon_token: _,
            bounds,
        }) = pred
        {
            if !lt_params.is_empty() && !bounds.is_empty() {
                let mut new_bounds = Punctuated::new();
                for bound in bounds.iter() {
                    if let syn::TypeParamBound::Trait(syn::TraitBound {
                        paren_token,
                        modifier,
                        lifetimes: _,
                        path,
                    }) = bound
                    {
                        let mut new_path = path.clone();
                        if !new_path.segments.is_empty() {
                            let last_segment = new_path.segments.last_mut().unwrap();
                            match &mut last_segment.arguments {
                                syn::PathArguments::AngleBracketed(args) => {
                                    if !args.args.is_empty() {
                                        args.args = Punctuated::from_iter(vec![syn::GenericArgument::Type(
                                            syn::Type::Verbatim(quote!(...).into()),
                                        )]);
                                    }
                                }
                                syn::PathArguments::Parenthesized(_) => {
                                    last_segment.arguments = syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![syn::GenericArgument::Type(
                                                syn::Type::Verbatim(quote!(...).into()),
                                            )]),
                                            gt_token: token::Gt::default(),
                                        },
                                    );
                                }
                                syn::PathArguments::None => {
                                    last_segment.arguments = syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![syn::GenericArgument::Type(
                                                syn::Type::Verbatim(quote!(...).into()),
                                            )]),
                                            gt_token: token::Gt::default(),
                                        },
                                    );
                                }
                            }
                        }
                        new_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: paren_token.clone(),
                            modifier: modifier.clone(),
                            lifetimes: None,
                            path: new_path,
                        }));
                    } else {
                        new_bounds.push(bound.clone());
                    }
                }
                *bounds = new_bounds;
            }
        }
        syn::visit_mut::visit_where_predicate_mut(self, pred);
    }
}