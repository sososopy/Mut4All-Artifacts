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

pub struct Add_Generic_Type_Parameter_150;

impl Mutator for Add_Generic_Type_Parameter_150 {
    fn name(&self) -> &str {
        "Add_Generic_Type_Parameter_150"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    generics.params.push(parse_quote!(T));
                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Path(type_path) = &**return_type {
                            let mut new_path = type_path.clone();
                            new_path.path.segments.push(syn::PathSegment {
                                ident: Ident::new("T", Span::call_site()),
                                arguments: syn::PathArguments::AngleBracketed(
                                    syn::AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: syn::punctuated::Punctuated::from_iter(vec![
                                            syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                qself: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: syn::punctuated::Punctuated::from_iter(
                                                        vec![syn::PathSegment {
                                                            ident: Ident::new("T", Span::call_site()),
                                                            arguments: syn::PathArguments::None,
                                                        }],
                                                    ),
                                                },
                                            })),
                                        ]),
                                        gt_token: token::Gt::default(),
                                    },
                                ),
                            });
                            *return_type = Box::new(Type::Path(new_path));
                        }
                    }
                }
            }
            else if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        generics.params.push(parse_quote!(T));
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                let mut new_path = type_path.clone();
                                new_path.path.segments.push(syn::PathSegment {
                                    ident: Ident::new("T", Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: syn::punctuated::Punctuated::from_iter(vec![
                                                syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                    qself: None,
                                                    path: syn::Path {
                                                        leading_colon: None,
                                                        segments: syn::punctuated::Punctuated::from_iter(
                                                            vec![syn::PathSegment {
                                                                ident: Ident::new("T", Span::call_site()),
                                                                arguments: syn::PathArguments::None,
                                                            }],
                                                        ),
                                                    },
                                                })),
                                            ]),
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                });
                                *return_type = Box::new(Type::Path(new_path));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a generic type parameter to non-main functions and methods, replacing their return types with a generic type. This transformation aims to test the compiler's handling of generic types and type inference."
    }
}