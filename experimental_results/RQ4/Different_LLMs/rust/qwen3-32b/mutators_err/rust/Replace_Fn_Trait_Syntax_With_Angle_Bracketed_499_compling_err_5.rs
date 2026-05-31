use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Fn_Trait_Syntax_With_Angle_Bracketed_499;

impl Mutator for Replace_Fn_Trait_Syntax_With_Angle_Bracketed_499 {
    fn name(&self) -> &str {
        "Replace_Fn_Trait_Syntax_With_Angle_Bracketed_499"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                let path = &mut trait_bound.path;
                                for segment in &mut path.segments {
                                    if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                        if let syn::PathArguments::Parenthesized(parens) = &segment.arguments {
                                            let inputs = &parens.inputs;
                                            let output = &parens.output;
                                            let mut generic_args = syn::punctuated::Punctuated::new();
                                            for input in inputs {
                                                generic_args.push(syn::GenericArgument::Type(input.clone()));
                                            }
                                            if let syn::ReturnType::Type(_, ty) = &*output {
                                                generic_args.push(syn::GenericArgument::Type((*ty).clone()));
                                            } else {
                                                generic_args.push(syn::GenericArgument::Type(syn::Type::Tuple(syn::TypeTuple {
                                                    paren_token: syn::token::Paren::default(),
                                                    elems: syn::punctuated::Punctuated::new(),
                                                })));
                                            }
                                            segment.arguments = syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: syn::token::Lt::default(),
                                                args: generic_args,
                                                gt_token: syn::token::Gt::default(),
                                            });
                                        }
                                    }
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