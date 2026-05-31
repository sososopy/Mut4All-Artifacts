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

pub struct Alter_Trait_Bounds_378;

impl Mutator for Alter_Trait_Bounds_378 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_378"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    let new_trait_bound = TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: Some(BoundLifetimes {
                            for_token: token::For {
                                span: Span::call_site(),
                            },
                            lt_token: token::Lt {
                                spans: [Span::call_site()],
                            },
                            lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                attrs: vec![],
                                lifetime: Lifetime {
                                    apostrophe: Span::call_site(),
                                    ident: Ident::new("'c", Span::call_site()),
                                },
                                colon_token: None,
                                bounds: Punctuated::new(),
                            })]),
                            gt_token: token::Gt {
                                spans: [Span::call_site()],
                            },
                        }),
                        path: SynPath {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: Ident::new("TraitC", Span::call_site()),
                                arguments: PathArguments::None,
                            }]),
                        },
                    };

                    where_clause.predicates.push(WherePredicate::Type(PredicateType {
                        lifetimes: None,
                        bounded_ty: Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("T", Span::call_site())),
                        }),
                        colon_token: token::Colon {
                            spans: [Span::call_site()],
                        },
                        bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}