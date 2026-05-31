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

pub struct Modify_Generic_Const_Expr_In_Fn_370;

impl Mutator for Modify_Generic_Const_Expr_In_Fn_370 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expr_In_Fn_370"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let mut new_segments = Punctuated::new();
                                    for segment in &trait_bound.path.segments {
                                        if let PathArguments::AngleBracketed(
                                            angle_bracketed_args,
                                        ) = &segment.arguments
                                        {
                                            let mut new_args = Punctuated::new();
                                            for arg in &angle_bracketed_args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    let new_expr: Expr = parse_quote! {
                                                        { |x| x * 2 + 3 }
                                                    };
                                                    new_args.push(GenericArgument::Const(new_expr));
                                                } else {
                                                    new_args.push(arg.clone());
                                                }
                                            }
                                            new_segments.push(PathSegment {
                                                ident: segment.ident.clone(),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: angle_bracketed_args
                                                            .colon2_token,
                                                        lt_token: angle_bracketed_args.lt_token,
                                                        args: new_args,
                                                        gt_token: angle_bracketed_args.gt_token,
                                                    },
                                                ),
                                            });
                                        } else {
                                            new_segments.push(segment.clone());
                                        }
                                    }
                                    trait_bound.path = syn::Path {
                                        leading_colon: trait_bound.path.leading_colon,
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
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic constant expressions in their where clauses. It replaces the const generic expression with a more complex closure-based expression, such as `{ |x| x * 2 + 3 }`. This transformation stresses the compiler's ability to handle intricate const evaluations within generic constraints, potentially revealing weaknesses in const evaluation or generic resolution."
    }
}