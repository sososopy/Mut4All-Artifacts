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

pub struct Alter_Type_Alias_Return_Type_697;

impl Mutator for Alter_Type_Alias_Return_Type_697 {
    fn name(&self) -> &str {
        "Alter_Type_Alias_Return_Type_697"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                    let new_trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                        paren_token: None,
                        modifier: syn::TraitBoundModifier::None,
                        lifetimes: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segs = Punctuated::new();
                                segs.push(syn::PathSegment {
                                    ident: Ident::new("AnotherTrait", Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: syn::punctuated::Punctuated::from_iter(
                                                type_impl_trait.bounds.iter().filter_map(|bound| {
                                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                                        Some(GenericArgument::Type(Type::Path(TypePath {
                                                            qself: None,
                                                            path: trait_bound.path.clone(),
                                                        })))
                                                    } else {
                                                        None
                                                    }
                                                }),
                                            ),
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                });
                                segs
                            },
                        },
                    });
                    type_alias.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds: Punctuated::from_iter(vec![new_trait_bound]),
                    }));
                }
            }
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let new_trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(syn::PathSegment {
                                        ident: Ident::new("AnotherTrait", Span::call_site()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    segs
                                },
                            },
                        });
                        *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl { span: Span::call_site() },
                            bounds: Punctuated::from_iter(vec![new_trait_bound]),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}