use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Return_Type_With_HigherRanked_Nested_ImplTrait_167;

impl Mutator for Replace_Return_Type_With_HigherRanked_Nested_ImplTrait_167 {
    fn name(&self) -> &str {
        "Replace_Return_Type_With_HigherRanked_Nested_ImplTrait_167"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                    if let Type::Path(_) = **return_type {
                        let new_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds: {
                                let mut bounds = Punctuated::new();
                                let outer_trait = TraitBound {
                                    modifier: TraitBoundModifier::For,
                                    lifetimes: Some(BoundLifetimes {
                                        for_token: Some(token::For {
                                            spans: Span::call_site(),
                                        }),
                                        lifetimes: {
                                            let mut lifetimes = Punctuated::new();
                                            lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                                attrs: vec![],
                                                lifetime: Lifetime::new("'a", Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            }));
                                            lifetimes
                                        },
                                        lt_token: Some(token::Lt {
                                            spans: Span::call_site(),
                                        }),
                                        gt_token: Some(token::Gt {
                                            spans: Span::call_site(),
                                        }),
                                    }),
                                    path: {
                                        let mut path = SynPath {
                                            leading_colon: None,
                                            segments: {
                                                let mut segments = Punctuated::new();
                                                let mut segment = PathSegment {
                                                    ident: Ident::new("Trait", Span::call_site()),
                                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                                                            let assoc_type = AssocType {
                                                                eq_token: token::Eq::default(),
                                                                ident: Ident::new("Assoc", Span::call_site()),
                                                                generics: None,
                                                                ty: Type::ImplTrait(TypeImplTrait {
                                                                    impl_token: token::Impl::default(),
                                                                    bounds: {
                                                                        let mut inner_bounds = Punctuated::new();
                                                                        inner_bounds.push(TypeParamBound::Trait(TraitBound {
                                                                            modifier: TraitBoundModifier::None,
                                                                            lifetimes: None,
                                                                            path: SynPath::from(Ident::new("Sized", Span::call_site())),
                                                                            paren_token: None,
                                                                        }));
                                                                        inner_bounds
                                                                    },
                                                                }),
                                                            };
                                                            args.push(GenericArgument::AssocType(assoc_type));
                                                            args
                                                        },
                                                        gt_token: token::Gt::default(),
                                                    }),
                                                };
                                                segments.push(segment);
                                                segments
                                            },
                                        };
                                        path
                                    },
                                    paren_token: None,
                                };
                                bounds.push(TypeParamBound::Trait(outer_trait));
                                bounds
                            },
                        });
                        *return_type = Box::new(new_type);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}