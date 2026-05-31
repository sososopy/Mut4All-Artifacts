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

pub struct Modify_Trait_Bound_In_Fn_Signature_50;

impl Mutator for Modify_Trait_Bound_In_Fn_Signature_50 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Fn_Signature_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("FnOnce") {
                                        trait_bound.path.segments.push(syn::PathSegment {
                                            ident: Ident::new("Output", Span::call_site()),
                                            arguments: syn::PathArguments::AngleBracketed(
                                                syn::AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: {
                                                        let mut args = Punctuated::new();
                                                        args.push(GenericArgument::Lifetime(
                                                            Lifetime::new("'a", Span::call_site()),
                                                        ));
                                                        args.push(GenericArgument::Type(
                                                            Type::Path(TypePath {
                                                                qself: None,
                                                                path: syn::Path::from(Ident::new(
                                                                    "i32",
                                                                    Span::call_site(),
                                                                )),
                                                            }),
                                                        ));
                                                        args
                                                    },
                                                    gt_token: token::Gt::default(),
                                                },
                                            ),
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies trait bounds in function signatures by adding additional trait constraints to existing bounds. This mutation targets higher-ranked trait bounds (HRTBs) to introduce new trait interactions, potentially exposing compiler bugs related to trait resolution and lifetime handling."
    }
}