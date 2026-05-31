use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_ImplTrait_Return_With_AsRef_Fn_161;

impl Mutator for Replace_ImplTrait_Return_With_AsRef_Fn_161 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_AsRef_Fn_161"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &**return_type {
                        // Create the reference type &()
                        let ref_type = syn::Type::Reference(syn::TypeReference {
                            and_token: token::And::default(),
                            lifetime: None,
                            elem: Box::new(syn::Type::Tuple(syn::TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: syn::punctuated::Punctuated::new(),
                            })),
                        });
                        // Create the Fn<&()> type
                        let fn_type = syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: {
                                    let mut segments = syn::punctuated::Punctuated::new();
                                    segments.push(syn::PathSegment {
                                        ident: Ident::new("Fn", proc_macro2::Span::call_site()),
                                        arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: {
                                                let mut args = syn::punctuated::Punctuated::new();
                                                args.push(syn::GenericArgument::Type(ref_type));
                                                args
                                            },
                                            gt_token: token::Gt::default(),
                                        }),
                                    });
                                    segments
                                },
                            },
                        });
                        // Create the AsRef<Fn<&()>> path
                        let as_ref_path = syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = syn::punctuated::Punctuated::new();
                                segments.push(syn::PathSegment {
                                    ident: Ident::new("AsRef", proc_macro2::Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: {
                                            let mut args = syn::punctuated::Punctuated::new();
                                            args.push(syn::GenericArgument::Type(fn_type));
                                            args
                                        },
                                        gt_token: token::Gt::default(),
                                    }),
                                });
                                segments
                            },
                        };
                        // Create the TraitBound for AsRef<Fn<&()>> 
                        let as_ref_trait = syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: as_ref_path,
                        };
                        // Create the new impl AsRef<Fn<&()>> return type
                        let new_type = syn::Type::ImplTrait(syn::TypeImplTrait {
                            impl_token: type_impl_trait.impl_token.clone(),
                            bounds: {
                                let mut bounds = syn::punctuated::Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(as_ref_trait));
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
        "The mutation operator targets functions returning `impl Trait` where the Trait is a standard library trait. It replaces the return type with `impl AsRef<Fn(&())>`, introducing a nested reference type within an opaque return. This forces the compiler to handle complex lifetime inference in opaque types, potentially triggering ICEs or type inference failures in the presence of nested references and trait bounds."
    }
}