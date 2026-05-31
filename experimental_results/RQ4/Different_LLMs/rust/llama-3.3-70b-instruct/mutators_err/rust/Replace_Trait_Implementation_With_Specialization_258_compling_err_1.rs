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

pub struct Replace_Trait_Implementation_With_Specialization_258;

impl Mutator for Replace_Trait_Implementation_With_Specialization_258 {
    fn name(&self) -> &str {
        "Replace_Trait_Implementation_With_Specialization_258"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = get_trait_path(&item_impl) {
                    let generic_impl = create_generic_impl(trait_path);
                    let specialized_impl = create_specialized_impl(trait_path, item_impl);
                    file.items.push(generic_impl);
                    file.items.push(specialized_impl);
                    *item = syn::Item::Empty(Empty {
                        span: Span::call_site(),
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait implementations with a generic implementation and a specialized implementation. This transformation aims to test the compiler's handling of trait specialization and may lead to bugs related to the specialization feature."
    }
}

fn get_trait_path(item_impl: &syn::ItemImpl) -> Option<(&syn::Path, syn::Path, &syn::PathArguments)> {
    if let syn::Impl {
        of_token,
        for_token,
        trait_,
        ..
    } = item_impl
    {
        if let Some(trait_path) = trait_ {
            return Some((of_token, trait_path.path.clone(), trait_path.path.segments.last().unwrap().arguments.clone()));
        }
    }
    None
}

fn create_generic_impl(trait_path: &syn::Path) -> syn::Item {
    let generic_params = syn::Generics {
        params: {
            let mut params = Punctuated::new();
            params.push(syn::GenericParam::Type(syn::TypeParam {
                attrs: vec![],
                ident: Ident::new("T", Span::call_site()),
                colon_token: Some(token::Colon::default()),
                bounds: Punctuated::new(),
                default: None,
                eq_token: None,
                super_token: None,
            }));
            syn::GenericsParams::AngleBracketed(syn::AngleBracketedGenericArguments {
                colon2_token: None,
                lt_token: token::Lt::default(),
                args: params,
                gt_token: token::Gt::default(),
            })
        },
        where_clause: None,
    };

    let trait_path_segment = trait_path.segments.last().unwrap();
    let trait_path_args = trait_path_segment.arguments.clone();

    syn::Item::Impl(syn::Impl {
        attrs: vec![],
        defaultness: None,
        unsafety: None,
        impl_token: token::Impl {
            span: Span::call_site(),
        },
        of_token: token::Of {
            span: Span::call_site(),
        },
        for_token: token::For {
            span: Span::call_site(),
        },
        generics: generic_params,
        trait_: Some(syn::Trait {
            path: syn::Path {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![syn::PathSegment {
                    ident: trait_path_segment.ident.clone(),
                    arguments: trait_path_args,
                }]),
            },
            additional_bounds: Punctuated::new(),
        }),
        self_ty: syn::Type::Path(syn::TypePath {
            qself: None,
            path: syn::Path {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![syn::PathSegment {
                    ident: Ident::new("T", Span::call_site()),
                    arguments: syn::PathArguments::None,
                }]),
            },
        }),
        items: Punctuated::new(),
    })
}

fn create_specialized_impl(trait_path: &syn::Path, item_impl: &syn::ItemImpl) -> syn::Item {
    let trait_path_segment = trait_path.segments.last().unwrap();
    let trait_path_args = trait_path_segment.arguments.clone();

    syn::Item::Impl(syn::Impl {
        attrs: vec![],
        defaultness: None,
        unsafety: None,
        impl_token: token::Impl {
            span: Span::call_site(),
        },
        of_token: token::Of {
            span: Span::call_site(),
        },
        for_token: token::For {
            span: Span::call_site(),
        },
        generics: syn::Generics {
            params: Punctuated::new(),
            where_clause: None,
        },
        trait_: Some(syn::Trait {
            path: syn::Path {
                leading_colon: None,
                segments: Punctuated::from_iter(vec![syn::PathSegment {
                    ident: trait_path_segment.ident.clone(),
                    arguments: trait_path_args,
                }]),
            },
            additional_bounds: Punctuated::new(),
        }),
        self_ty: item_impl.self_ty.clone(),
        items: item_impl.items.clone(),
    })
}