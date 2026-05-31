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

pub struct Alter_Lifetime_Bounds_689;

impl Mutator for Alter_Lifetime_Bounds_689 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Bounds_689"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let mut has_lifetime_bound = false;
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.segments.iter().any(|seg| {
                                            seg.ident == "Debug" && trait_bound.lifetimes.is_some()
                                        }) {
                                            has_lifetime_bound = true;
                                            break;
                                        }
                                    }
                                }
                                if has_lifetime_bound {
                                    method.sig.generics.params.clear();
                                    method.sig.output = ReturnType::Type(
                                        Default::default(),
                                        Box::new(Type::ImplTrait(TypeImplTrait {
                                            impl_token: token::Impl {
                                                span: Span::call_site(),
                                            },
                                            bounds: {
                                                let mut bounds = Punctuated::new();
                                                bounds.push(TypeParamBound::Trait(TraitBound {
                                                    paren_token: None,
                                                    modifier: TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: syn::Path {
                                                        leading_colon: None,
                                                        segments: {
                                                            let mut segs = Punctuated::new();
                                                            segs.push(syn::PathSegment {
                                                                ident: Ident::new(
                                                                    "Debug",
                                                                    Span::call_site(),
                                                                ),
                                                                arguments: syn::PathArguments::None,
                                                            });
                                                            segs
                                                        },
                                                    },
                                                }));
                                                bounds
                                            },
                                        })),
                                    );
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let mut has_lifetime_bound = false;
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.segments.iter().any(|seg| {
                                            seg.ident == "Debug" && trait_bound.lifetimes.is_some()
                                        }) {
                                            has_lifetime_bound = true;
                                            break;
                                        }
                                    }
                                }
                                if has_lifetime_bound {
                                    method.sig.generics.params.clear();
                                    method.sig.output = ReturnType::Type(
                                        Default::default(),
                                        Box::new(Type::ImplTrait(TypeImplTrait {
                                            impl_token: token::Impl {
                                                span: Span::call_site(),
                                            },
                                            bounds: {
                                                let mut bounds = Punctuated::new();
                                                bounds.push(TypeParamBound::Trait(TraitBound {
                                                    paren_token: None,
                                                    modifier: TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: syn::Path {
                                                        leading_colon: None,
                                                        segments: {
                                                            let mut segs = Punctuated::new();
                                                            segs.push(syn::PathSegment {
                                                                ident: Ident::new(
                                                                    "Debug",
                                                                    Span::call_site(),
                                                                ),
                                                                arguments: syn::PathArguments::None,
                                                            });
                                                            segs
                                                        },
                                                    },
                                                }));
                                                bounds
                                            },
                                        })),
                                    );
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