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

pub struct Add_Unused_Lifetime_Bounds_386;

impl Mutator for Add_Unused_Lifetime_Bounds_386 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Bounds_386"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: unused_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));

                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        type_path.path.segments.push(PathSegment {
                            ident: Ident::new("PhantomData", Span::call_site()),
                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: token::Lt::default(),
                                args: Punctuated::from_iter(vec![GenericArgument::Lifetime(unused_lifetime)]),
                                gt_token: token::Gt::default(),
                            }),
                        });
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: unused_lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        method.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));

                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(type_path) = &mut **ty {
                                type_path.path.segments.push(PathSegment {
                                    ident: Ident::new("PhantomData", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: Punctuated::from_iter(vec![GenericArgument::Lifetime(unused_lifetime)]),
                                        gt_token: token::Gt::default(),
                                    }),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter to function and method signatures and incorporates it into the return type using `PhantomData`. This transformation introduces unnecessary complexity in lifetime management, potentially leading to ICEs or other compiler issues related to lifetime resolution and variance analysis."
    }
}