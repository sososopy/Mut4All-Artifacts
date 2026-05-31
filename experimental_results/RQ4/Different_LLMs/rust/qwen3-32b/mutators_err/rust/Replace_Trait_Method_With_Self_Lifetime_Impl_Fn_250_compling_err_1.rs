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

pub struct Replace_Trait_Method_With_Self_Lifetime_Impl_Fn_250;

impl Mutator for Replace_Trait_Method_With_Self_Lifetime_Impl_Fn_250 {
    fn name(&self) -> &str {
        "Replace_Trait_Method_With_Self_Lifetime_Impl_Fn_250"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        // Add self-bounded lifetime parameter
                        method.sig.generics.params.push(parse_quote!(lifetime 'a: 'a));
                        
                        // Replace return type with impl Sized + FnOnce<()>
                        if let syn::ReturnType::Type(_, ref mut return_type) = method.sig.output {
                            *return_type = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                                impl_token: token::Impl::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    // Add Sized trait bound
                                    bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segs = Punctuated::new();
                                                segs.push(syn::PathSegment {
                                                    ident: Ident::new("Sized", Span::call_site()),
                                                    arguments: syn::PathArguments::None,
                                                });
                                                segs
                                            },
                                        },
                                    }));
                                    // Add FnOnce<()> trait bound
                                    let fn_once_path = syn::Path {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            let args = AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: Punctuated::from_iter(vec![GenericArgument::Type(
                                                    Type::Tuple(TypeTuple {
                                                        paren_token: Paren::default(),
                                                        elems: Punctuated::new(),
                                                    }),
                                                )]),
                                                gt_token: token::Gt::default(),
                                            };
                                            segs.push(syn::PathSegment {
                                                ident: Ident::new("FnOnce", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(args),
                                            });
                                            segs
                                        },
                                    };
                                    bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: fn_once_path,
                                    }));
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
        ""
    }
}