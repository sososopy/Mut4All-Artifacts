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

pub struct Replace_Return_Type_with_impl_Trait_444;

impl Mutator for Replace_Return_Type_with_impl_Trait_444 {
    fn name(&self) -> &str {
        "Replace_Return_Type_with_impl_Trait_444"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut used_lifetimes: HashSet<String> = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut lifetime: syn::Lifetime = parse_quote!('a0);
                let mut idx = 0;
                loop {
                    let name = format!("a{}", idx);
                    if used_lifetimes.insert(format!("'{}", name)) {
                        lifetime = syn::Lifetime {
                            apostrophe: proc_macro2::Span::call_site(),
                            ident: syn::Ident::new(&name, proc_macro2::Span::call_site()),
                        };
                        break;
                    }
                    idx += 1;
                }

                let sized_bound = TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: Some(BoundLifetimes {
                        for_token: Default::default(),
                        lt_token: Default::default(),
                        lifetimes: {
                            let mut punct = Punctuated::new();
                            punct.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: vec![],
                                lifetime: lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                            punct
                        },
                        gt_token: Default::default(),
                    }),
                    path: syn::Path::from(syn::Ident::new("Sized", proc_macro2::Span::call_site())),
                });

                let clone_bound = TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path::from(syn::Ident::new("Clone", proc_macro2::Span::call_site())),
                });

                func.sig.output = ReturnType::Type(
                    syn::token::RArrow::default(),
                    Box::new(Type::ImplTrait(TypeImplTrait {
                        impl_token: Default::default(),
                        bounds: {
                            let mut bounds = syn::punctuated::Punctuated::new();
                            bounds.push(sized_bound);
                            bounds.push(clone_bound);
                            bounds
                        },
                    })),
                );
            }
            if let syn::Item::Impl(tag1) = item {
                for impl_item in &mut tag1.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ty) = &mut method.sig.output {
                            let mut lifetime: syn::Lifetime = parse_quote!('a0);
                            let mut idx = 0;
                            loop {
                                let name = format!("a{}", idx);
                                if used_lifetimes.insert(format!("'{}", name)) {
                                    lifetime = syn::Lifetime {
                                        apostrophe: proc_macro2::Span::call_site(),
                                        ident: syn::Ident::new(&name, proc_macro2::Span::call_site()),
                                    };
                                    break;
                                }
                                idx += 1;
                            }
                            let sized_bound = TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: Some(BoundLifetimes {
                                    for_token: Default::default(),
                                    lt_token: Default::default(),
                                    lifetimes: {
                                        let mut punct = Punctuated::new();
                                        punct.push(GenericParam::Lifetime(LifetimeParam {
                                            attrs: vec![],
                                            lifetime: lifetime.clone(),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                        punct
                                    },
                                    gt_token: Default::default(),
                                }),
                                path: syn::Path::from(syn::Ident::new("Sized", proc_macro2::Span::call_site())),
                            });
                            let clone_bound = TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: syn::Path::from(syn::Ident::new("Clone", proc_macro2::Span::call_site())),
                            });
                            *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(sized_bound);
                                    bounds.push(clone_bound);
                                    bounds
                                },
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Modify the return type of the function to a nested RPIT(returning impl Trait) type with a lifecycle"
    }
}