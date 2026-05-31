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

pub struct Introduce_HRTB_And_Impl_Trait_30;

impl Mutator for Introduce_HRTB_And_Impl_Trait_30 {
    fn name(&self) -> &str {
        "Introduce_HRTB_And_Impl_Trait_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }

                // Introduce higher-ranked trait bounds
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            let new_bounds: Punctuated<TypeParamBound, Plus> = type_impl_trait
                                .bounds
                                .iter()
                                .filter_map(|bound| {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        Some(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: Some(BoundLifetimes {
                                                for_token: token::For {
                                                    span: Span::call_site(),
                                                },
                                                lt_token: token::Lt {
                                                    spans: [Span::call_site()],
                                                },
                                                lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                                    attrs: vec![],
                                                    lifetime: Lifetime::new("'a", Span::call_site()),
                                                    colon_token: None,
                                                    bounds: Punctuated::new(),
                                                }]),
                                                gt_token: token::Gt {
                                                    spans: [Span::call_site()],
                                                },
                                            }),
                                            path: trait_bound.path.clone(),
                                        }))
                                    } else {
                                        None
                                    }
                                })
                                .collect();

                            type_impl_trait.bounds = new_bounds;
                        }
                    }
                }

                // Modify return type if it uses `impl Trait`
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let new_bounds: Punctuated<TypeParamBound, Plus> = type_impl_trait
                            .bounds
                            .iter()
                            .filter_map(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    Some(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: Some(BoundLifetimes {
                                            for_token: token::For {
                                                span: Span::call_site(),
                                            },
                                            lt_token: token::Lt {
                                                spans: [Span::call_site()],
                                            },
                                            lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                                attrs: vec![],
                                                lifetime: Lifetime::new("'b", Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            }]),
                                            gt_token: token::Gt {
                                                spans: [Span::call_site()],
                                            },
                                        }),
                                        path: trait_bound.path.clone(),
                                    }))
                                } else {
                                    None
                                }
                            })
                            .collect();

                        type_impl_trait.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}