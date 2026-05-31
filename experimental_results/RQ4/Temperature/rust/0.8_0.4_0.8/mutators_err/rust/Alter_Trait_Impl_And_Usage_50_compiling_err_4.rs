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

pub struct Alter_Trait_Impl_And_Usage_50;

impl Mutator for Alter_Trait_Impl_And_Usage_50 {
    fn name(&self) -> &str {
        "Alter_Trait_Impl_And_Usage_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.path.segments.last().unwrap().ident == "F" {
                                    predicate_type.bounds.push(syn::TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: Some(BoundLifetimes {
                                            for_token: token::For { span: Span::call_site() },
                                            lt_token: token::Lt { spans: [Span::call_site()] },
                                            lifetimes: Punctuated::from_iter(vec![Lifetime {
                                                apostrophe: Span::call_site(),
                                                ident: Ident::new("'a", Span::call_site()),
                                            }]),
                                            gt_token: token::Gt { spans: [Span::call_site()] },
                                        }),
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![
                                                PathSegment {
                                                    ident: Ident::new("SomeTrait", Span::call_site()),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt { spans: [Span::call_site()] },
                                                            args: Punctuated::from_iter(vec![
                                                                GenericArgument::Lifetime(Lifetime {
                                                                    apostrophe: Span::call_site(),
                                                                    ident: Ident::new("'a", Span::call_site()),
                                                                }),
                                                            ]),
                                                            gt_token: token::Gt { spans: [Span::call_site()] },
                                                        }
                                                    ),
                                                }
                                            ]),
                                        },
                                    }));
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