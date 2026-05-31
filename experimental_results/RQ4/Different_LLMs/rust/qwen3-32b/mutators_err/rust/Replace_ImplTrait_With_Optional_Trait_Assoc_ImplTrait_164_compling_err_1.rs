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

pub struct Replace_ImplTrait_With_Optional_Trait_Assoc_ImplTrait_164;

impl Mutator for Replace_ImplTrait_With_Optional_Trait_Assoc_ImplTrait_164 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_With_Optional_Trait_Assoc_ImplTrait_164"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(_) = &**return_type {
                        let inner_impl = syn::TypeImplTrait {
                            impl_token: syn::token::Impl::default(),
                            bounds: {
                                let mut inner_bounds = syn::punctuated::Punctuated::new();
                                let another_trait_path = syn::parse_quote!(AnotherTrait);
                                inner_bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: another_trait_path,
                                }));
                                inner_bounds
                            },
                        };
                        let assoc_type = syn::GenericArgument::AssocType(syn::AssocType {
                            eq_token: syn::token::Eq::default(),
                            ident: syn::Ident::new("AssociatedType", proc_macro2::Span::call_site()),
                            gen_args: None,
                            colon_token: None,
                            ty: Box::new(syn::Type::ImplTrait(inner_impl)),
                        });
                        let trait_path = syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = syn::punctuated::Punctuated::new();
                                segments.push(syn::PathSegment {
                                    ident: syn::Ident::new("Trait", proc_macro2::Span::call_site()),
                                    arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: syn::token::Lt::default(),
                                        args: {
                                            let mut args = syn::punctuated::Punctuated::new();
                                            args.push(assoc_type);
                                            args
                                        },
                                        gt_token: syn::token::Gt::default(),
                                    }),
                                });
                                segments
                            },
                        };
                        let outer_trait_bound = syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::Question,
                            lifetimes: None,
                            path: trait_path,
                        };
                        let outer_impl_trait = syn::TypeImplTrait {
                            impl_token: syn::token::Impl::default(),
                            bounds: {
                                let mut bounds = syn::punctuated::Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(outer_trait_bound));
                                bounds
                            },
                        };
                        *return_type = Box::new(syn::Type::ImplTrait(outer_impl_trait));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}