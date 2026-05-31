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

pub struct Modify_Generic_Const_Expr_In_Fn_Sig_434;

impl Mutator for Modify_Generic_Const_Expr_In_Fn_Sig_434 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expr_In_Fn_Sig_434"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(pred_type) = predicate {
                            for bound in &mut pred_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.segments.len() == 1 {
                                        let segment = trait_bound.path.segments.first().unwrap();
                                        if segment.ident == "for" {
                                            if let syn::PathArguments::AngleBracketed(
                                                angle_args,
                                            ) = &segment.arguments
                                            {
                                                let mut new_args = Punctuated::new();
                                                for arg in &angle_args.args {
                                                    if let syn::GenericArgument::Const(
                                                        const_expr,
                                                    ) = arg
                                                    {
                                                        if let syn::Expr::Lit(expr_lit) =
                                                            const_expr
                                                        {
                                                            if let syn::Lit::Int(lit_int) =
                                                                &expr_lit.lit
                                                            {
                                                                let new_value = match lit_int
                                                                    .base10_parse::<usize>()
                                                                {
                                                                    Ok(v) => v + 1,
                                                                    Err(_) => 0,
                                                                };
                                                                let new_lit = syn::Lit::Int(
                                                                    syn::LitInt::new(
                                                                        &new_value.to_string(),
                                                                        lit_int.span(),
                                                                    ),
                                                                );
                                                                let new_expr = syn::Expr::Lit(
                                                                    syn::ExprLit {
                                                                        attrs: Vec::new(),
                                                                        lit: new_lit,
                                                                    },
                                                                );
                                                                new_args.push(
                                                                    syn::GenericArgument::Const(
                                                                        new_expr,
                                                                    ),
                                                                );
                                                            }
                                                        } else {
                                                            new_args.push(arg.clone());
                                                        }
                                                    } else {
                                                        new_args.push(arg.clone());
                                                    }
                                                }
                                                let new_segment = syn::PathSegment {
                                                    ident: segment.ident.clone(),
                                                    arguments:
                                                        syn::PathArguments::AngleBracketed(
                                                            syn::AngleBracketedGenericArguments {
                                                                colon2_token: angle_args
                                                                    .colon2_token,
                                                                lt_token: angle_args.lt_token,
                                                                args: new_args,
                                                                gt_token: angle_args.gt_token,
                                                            },
                                                        ),
                                                };
                                                let mut new_segments = Punctuated::new();
                                                new_segments.push(new_segment);
                                                trait_bound.path = syn::Path {
                                                    leading_colon: trait_bound
                                                        .path
                                                        .leading_colon,
                                                    segments: new_segments,
                                                };
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
        "This mutator targets functions with generic constant expressions in their `where` clauses. It specifically looks for `for<const N: usize = X, ...>` patterns and increments the constant value `X` by 1. This transformation tests the compiler's ability to handle changes in generic constant expressions, potentially exposing issues in const evaluation, generic resolution, and trait bound checking."
    }
}