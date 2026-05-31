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

pub struct Replace_Lifetime_Parameters_With_Non_Universal_Region_Args_195;

impl Mutator for Replace_Lifetime_Parameters_With_Non_Universal_Region_Args_195 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameters_With_Non_Universal_Region_Args_195"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut new_generics = func.sig.generics.clone();
                let mut new_params = Punctuated::new();
                for param in new_generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        let lifetime = Lifetime::new("'a", Span::call_site());
                        new_params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: lifetime_param.attrs.clone(),
                            lifetime: lifetime,
                            colon_token: lifetime_param.colon_token,
                        }));
                    } else {
                        new_params.push(param);
                    }
                }
                new_generics.params = new_params;
                func.sig.generics = new_generics;
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            let mut new_path = type_path.clone();
                            new_path.path.segments.push(PathSegment {
                                ident: Ident::new("a", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: Punctuated::from_iter(vec![GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site()))]),
                                    gt_token: token::Gt::default(),
                                }),
                            });
                            pat_type.ty = Box::new(Type::Path(new_path));
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let mut new_generics = func.sig.generics.clone();
                        let mut new_params = Punctuated::new();
                        for param in new_generics.params {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                let lifetime = Lifetime::new("'a", Span::call_site());
                                new_params.push(GenericParam::Lifetime(LifetimeParam {
                                    attrs: lifetime_param.attrs.clone(),
                                    lifetime: lifetime,
                                    colon_token: lifetime_param.colon_token,
                                }));
                            } else {
                                new_params.push(param);
                            }
                        }
                        new_generics.params = new_params;
                        func.sig.generics = new_generics;
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    let mut new_path = type_path.clone();
                                    new_path.path.segments.push(PathSegment {
                                        ident: Ident::new("a", Span::call_site()),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site()))]),
                                            gt_token: token::Gt::default(),
                                        }),
                                    });
                                    pat_type.ty = Box::new(Type::Path(new_path));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetime parameters in function and method definitions with non-universal region arguments. This transformation introduces non-universal region arguments, increasing the likelihood of triggering bugs related to lifetime inference and region checking."
    }
}