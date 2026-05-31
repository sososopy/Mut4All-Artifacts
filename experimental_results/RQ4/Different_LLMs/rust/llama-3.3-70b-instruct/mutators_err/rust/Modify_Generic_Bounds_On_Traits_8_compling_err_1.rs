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

pub struct Modify_Generic_Bounds_On_Traits_8;

impl Mutator for Modify_Generic_Bounds_On_Traits_8 {
    fn name(&self) -> &str {
        "Modify_Generic_Bounds_On_Traits_8"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for generic_param in &mut trait_item.generics.params {
                    if let GenericParam::Type(type_param) = generic_param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_bound = TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    modifier: trait_bound.modifier,
                                    lifetimes: trait_bound.lifetimes,
                                    path: SynPath {
                                        leading_colon: trait_bound.path.leading_colon,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(SynPathSegment {
                                                ident: Ident::new("Debug", Span::call_site()),
                                                arguments: PathArguments::None,
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
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        for bound in &mut type_item.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_bound = TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    modifier: trait_bound.modifier,
                                    lifetimes: trait_bound.lifetimes,
                                    path: SynPath {
                                        leading_colon: trait_bound.path.leading_colon,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(SynPathSegment {
                                                ident: Ident::new("Send", Span::call_site()),
                                                arguments: PathArguments::None,
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions and modifies the generic bounds specified for associated types or methods. It introduces new trait bounds, such as Debug or Send, to test the compiler's handling of generic bounds on traits and their associated types or methods."
    }
}