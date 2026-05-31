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

pub struct Add_Higher_Rank_Async_Bounds_474;

impl Mutator for Add_Higher_Rank_Async_Bounds_474 {
    fn name(&self) -> &str {
        "Add_Higher_Rank_Async_Bounds_474"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for generic in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = generic {
                        let bounds = &mut type_param.bounds;
                        let mut modified = false;
                        let mut new_bounds = Punctuated::new();
                        for bound in bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let is_fn = trait_bound.path.segments.iter().any(|seg| {
                                    seg.ident == "Fn" || seg.ident == "FnMut" || seg.ident == "FnOnce"
                                });
                                if is_fn {
                                    modified = true;
                                    if let PathArguments::Parenthesized(paren_args) = &trait_bound.path.segments[0].arguments {
                                        let mut new_args = paren_args.clone();
                                        let original_return_type = if let Some(ReturnType::Type(_, ty)) = &paren_args.output {
                                            ty.clone()
                                        } else {
                                            parse_quote!(()).into()
                                        };
                                        new_args.output = Some(ReturnType::Type(
                                            Token![->](Span::call_site()),
                                            Box::new(parse_quote!(std::future::Future<Output = #original_return_type>)),
                                        ));
                                        let mut new_path = trait_bound.path.clone();
                                        let first_segment = new_path.segments.first_mut().unwrap();
                                        first_segment.arguments = PathArguments::Parenthesized(new_args);
                                        let new_trait_bound = TraitBound {
                                            paren_token: trait_bound.paren_token.clone(),
                                            modifier: trait_bound.modifier,
                                            lifetimes: Some(BoundLifetimes {
                                                for_token: Token![for](Span::call_site()),
                                                lt_token: Token![<](Span::call_site()),
                                                lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                                    attrs: Vec::new(),
                                                    lifetime: Lifetime::new("'a", Span::call_site()),
                                                    colon_token: None,
                                                    bounds: Punctuated::new(),
                                                })]),
                                                gt_token: Token![>](Span::call_site()),
                                            }),
                                            path: new_path,
                                        };
                                        new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        if modified {
                            *bounds = new_bounds;
                            let mut has_send = false;
                            let mut has_sync = false;
                            for bound in bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let is_send = trait_bound.path.segments.iter().any(|seg| seg.ident == "Send");
                                    let is_sync = trait_bound.path.segments.iter().any(|seg| seg.ident == "Sync");
                                    if is_send {
                                        has_send = true;
                                    }
                                    if is_sync {
                                        has_sync = true;
                                    }
                                }
                            }
                            if !has_send {
                                bounds.push(parse_quote!(Send));
                            }
                            if !has_sync {
                                bounds.push(parse_quote!(Sync));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms generic parameters with Fn, FnMut, or FnOnce trait bounds into higher-rank async trait bounds. It replaces the return type with a Future<Output = ...>, adds for<'a> lifetime quantification, and appends Send and Sync bounds if not already present. This challenges the compiler's handling of async trait bounds, higher-rank lifetimes, and trait object safety constraints."
    }
}