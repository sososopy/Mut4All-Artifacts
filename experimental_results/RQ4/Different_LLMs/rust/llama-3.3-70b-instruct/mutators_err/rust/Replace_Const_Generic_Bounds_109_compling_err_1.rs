use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Const_Generic_Bounds_109;

impl Mutator for Replace_Const_Generic_Bounds_109 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bounds_109"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident ==main {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Some(bounds) = &param.bounds {
                            let mut new_bounds = Punctuated::new();
                            for bound in bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_trait_bound = TraitBound {
                                        paren_token: trait_bound.paren_token,
                                        modifier: trait_bound.modifier,
                                        lifetimes: trait_bound.lifetimes,
                                        path: SynPath {
                                            leading_colon: trait_bound.path.leading_colon,
                                            segments: {
                                                let mut segments = Punctuated::new();
                                                for segment in trait_bound.path.segments {
                                                    if segment.ident ==Copy {
                                                        segments.push(PathSegment {
                                                            ident: Ident::new("Clone", segment.ident.span()),
                                                            arguments: segment.arguments,
                                                        });
                                                    } else {
                                                        segments.push(segment);
                                                    }
                                                }
                                                segments
                                            },
                                        },
                                    };
                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            param.bounds = Some(new_bounds);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(param) = param {
                                if let Some(bounds) = &param.bounds {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            let new_trait_bound = TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier,
                                                lifetimes: trait_bound.lifetimes,
                                                path: SynPath {
                                                    leading_colon: trait_bound.path.leading_colon,
                                                    segments: {
                                                        let mut segments = Punctuated::new();
                                                        for segment in trait_bound.path.segments {
                                                            if segment.ident ==Copy {
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new("Clone", segment.ident.span()),
                                                                    arguments: segment.arguments,
                                                                });
                                                            } else {
                                                                segments.push(segment);
                                                            }
                                                        }
                                                        segments
                                                    },
                                                },
                                            };
                                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    }
                                    param.bounds = Some(new_bounds);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing bounds on const generic parameters with alternative bounds, potentially altering the constraints on the generic parameters. It aims to test the compiler's handling of const generics and their bounds, and may lead to ICEs or inference failures in the trait resolution system."
    }
}