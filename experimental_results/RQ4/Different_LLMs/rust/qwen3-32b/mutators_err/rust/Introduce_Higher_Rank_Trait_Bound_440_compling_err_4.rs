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

pub struct Introduce_Higher_Rank_Trait_Bound_440;

impl Mutator for Introduce_Higher_Rank_Trait_Bound_440 {
    fn name(&self) -> &str {
        "Introduce_Higher_Rank_Trait_Bound_440"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(type_pred) = predicate {
                            for bound in &mut type_pred.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Fn") {
                                        let new_trait_bound = syn::TraitBound {
                                            modifier: syn::TraitBoundModifier::For(LifetimeParam::new(Lifetime::new("'a", Span::call_site()))),
                                            path: {
                                                let mut path = trait_bound.path.clone();
                                                let mut args = syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: syn::token::Lt::default(),
                                                    args: {
                                                        let mut args = syn::punctuated::Punctuated::new();
                                                        let lifetime = Lifetime::new("'a", Span::call_site());
                                                        let ref_type = Type::Reference(Box::new(syn::TypeReference {
                                                            and_token: Default::default(),
                                                            lifetimes: Some(syn::punctuated::Punctuated::from_iter([lifetime.clone()])),
                                                            elem: Box::new(syn::Type::Path(syn::TypePath {
                                                                qself: None,
                                                                path: syn::Path::from(syn::Ident::new("T", Span::call_site())),
                                                            })),
                                                        }));
                                                        args.push(syn::GenericArgument::Type(ref_type));
                                                        args
                                                    },
                                                    gt_token: syn::token::Gt::default(),
                                                });
                                                let mut segments = path.segments;
                                                if let Some(first_segment) = segments.iter_mut().next() {
                                                    first_segment.arguments = args;
                                                }
                                                path
                                            },
                                            ..trait_bound.clone()
                                        };
                                        *bound = syn::TypeParamBound::Trait(new_trait_bound);
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