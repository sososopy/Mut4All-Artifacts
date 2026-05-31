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

pub struct Replace_Iter_Return_With_Item_Type_12;

impl Mutator for Replace_Iter_Return_With_Item_Type_12 {
    fn name(&self) -> &str {
        "Replace_Iter_Return_With_Item_Type_12"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::Path(type_path) = &**return_type {
                        let segments = &type_path.path.segments;
                        if segments.len() >= 3 {
                            let first = &segments[0].ident;
                            let second = &segments[1].ident;
                            let third = &segments[2].ident;
                            if first == "std" && second == "slice" && (third == "Iter" || third == "IterMut") {
                                if let syn::PathArguments::AngleBracketed(args) = &segments[2].arguments {
                                    if args.args.len() == 1 {
                                        if let syn::GenericArgument::Type(ty) = args.args.first().unwrap() {
                                            let eq_token = parse_quote!(=);
                                            let assoc_type = syn::TypeParam {
                                                ident: Ident::new("Item", third.ident.span()),
                                                colon2_token: None,
                                                bounds: Punctuated::new(),
                                                eq_token: Some(eq_token),
                                                default: Some(ty.clone()),
                                                attrs: Vec::new(),
                                            };
                                            let new_args = syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: args.lt_token.clone(),
                                                args: Punctuated::from_iter(vec![GenericArgument::AssocType(assoc_type)]),
                                                gt_token: args.gt_token.clone(),
                                            };
                                            let mut new_segment = segments[2].clone();
                                            new_segment.arguments = PathArguments::AngleBracketed(new_args);
                                            let mut new_segments = segments.clone();
                                            new_segments[2] = new_segment;
                                            let new_path = syn::Path {
                                                leading_colon: type_path.path.leading_colon.clone(),
                                                segments: new_segments,
                                            };
                                            *return_type = Box::new(syn::Type::Path(syn::TypePath {
                                                qself: type_path.qself.clone(),
                                                path: new_path,
                                            }));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        if let syn::ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let syn::Type::Path(type_path) = &**return_type {
                                let segments = &type_path.path.segments;
                                if segments.len() >= 3 {
                                    let first = &segments[0].ident;
                                    let second = &segments[1].ident;
                                    let third = &segments[2].ident;
                                    if first == "std" && second == "slice" && (third == "Iter" || third == "IterMut") {
                                        if let syn::PathArguments::AngleBracketed(args) = &segments[2].arguments {
                                            if args.args.len() == 1 {
                                                if let syn::GenericArgument::Type(ty) = args.args.first().unwrap() {
                                                    let eq_token = parse_quote!(=);
                                                    let assoc_type = syn::TypeParam {
                                                        ident: Ident::new("Item", third.ident.span()),
                                                        colon2_token: None,
                                                        bounds: Punctuated::new(),
                                                        eq_token: Some(eq_token),
                                                        default: Some(ty.clone()),
                                                        attrs: Vec::new(),
                                                    };
                                                    let new_args = syn::AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: args.lt_token.clone(),
                                                        args: Punctuated::from_iter(vec![GenericArgument::AssocType(assoc_type)]),
                                                        gt_token: args.gt_token.clone(),
                                                    };
                                                    let mut new_segment = segments[2].clone();
                                                    new_segment.arguments = PathArguments::AngleBracketed(new_args);
                                                    let mut new_segments = segments.clone();
                                                    new_segments[2] = new_segment;
                                                    let new_path = syn::Path {
                                                        leading_colon: type_path.path.leading_colon.clone(),
                                                        segments: new_segments,
                                                    };
                                                    *return_type = Box::new(syn::Type::Path(syn::TypePath {
                                                        qself: type_path.qself.clone(),
                                                        path: new_path,
                                                    }));
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}