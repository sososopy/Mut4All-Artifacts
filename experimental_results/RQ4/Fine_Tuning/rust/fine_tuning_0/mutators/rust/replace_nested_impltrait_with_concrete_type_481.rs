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

pub struct Replace_Nested_ImplTrait_With_Concrete_Type_481;

impl Mutator for Replace_Nested_ImplTrait_With_Concrete_Type_481 {
    fn name(&self) -> &str {
        "Replace_Nested_ImplTrait_With_Concrete_Type_481"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(type_alias) = item {
                if let syn::Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    let mut new_bounds = Punctuated::new();
                    for bound in &type_impl_trait.bounds {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            if trait_bound.path.segments.last().unwrap().ident == "Iterator" {
                                let mut new_segments = Punctuated::new();
                                new_segments.push(syn::PathSegment {
                                    ident: Ident::new("std", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                });
                                new_segments.push(syn::PathSegment {
                                    ident: Ident::new("vec", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                });
                                new_segments.push(syn::PathSegment {
                                    ident: Ident::new("IntoIter", Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                                })));
                                                args
                                            },
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                });
                                let new_trait_bound = syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: new_segments,
                                    },
                                };
                                new_bounds.push(syn::TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        } else {
                            new_bounds.push(bound.clone());
                        }
                    }
                    type_alias.ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: new_bounds,
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type aliases using `impl Trait` with nested `impl Trait`. It replaces the inner `impl Trait` with a concrete iterator type `std::vec::IntoIter<usize>`. This transformation maintains the outer trait bound expectations while eliminating nested `impl Trait`, potentially exposing bugs in type resolution and aliasing."
    }
}