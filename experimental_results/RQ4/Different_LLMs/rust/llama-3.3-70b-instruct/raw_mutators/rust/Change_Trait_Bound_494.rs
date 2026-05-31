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

pub struct Change_Trait_Bound_494;

impl Mutator for Change_Trait_Bound_494 {
    fn name(&self) -> &str {
        "Change_Trait_Bound_494"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(generics) = &mut func.sig.generics {
                            for param in &mut generics.params {
                                if let GenericParam::Type(type_param) = param {
                                    for bound in &mut type_param.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(paren_token) = &trait_bound.paren_token {
                                                let new_trait_bound = TraitBound {
                                                    paren_token: Some(token::Paren {
                                                        span: paren_token.span,
                                                    }),
                                                    modifier: trait_bound.modifier,
                                                    lifetimes: trait_bound.lifetimes,
                                                    path: syn::Path {
                                                        leading_colon: trait_bound.path.leading_colon,
                                                        segments: {
                                                            let mut segments = Punctuated::new();
                                                            segments.push(syn::PathSegment {
                                                                ident: Ident::new(
                                                                    "FnMut",
                                                                    Span::call_site(),
                                                                ),
                                                                arguments: trait_bound.path.segments
                                                                    .first()
                                                                    .unwrap()
                                                                    .arguments
                                                                    .clone(),
                                                            });
                                                            segments
                                                        },
                                                    },
                                                };
                                                *bound = TypeParamBound::Trait(new_trait_bound);
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
        "The mutation operator targets trait definitions with higher-ranked trait bounds and changes the trait bound to a different bound, such as changing a bound from `Fn(&'a ())` to `FnMut(&'a ())`. This transformation aims to test the compiler's handling of higher-ranked trait bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}