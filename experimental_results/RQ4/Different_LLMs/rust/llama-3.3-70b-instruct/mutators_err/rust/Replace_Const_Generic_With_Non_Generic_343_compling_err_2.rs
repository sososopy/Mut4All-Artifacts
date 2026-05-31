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

pub struct Replace_Const_Generic_With_Non_Generic_343;

impl Mutator for Replace_Const_Generic_With_Non_Generic_343 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Generic_343"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if let Some(const_generics) = trait_item.generics.params {
                    let mut new_generics = trait_item.generics.clone();
                    new_generics.params = Punctuated::new();
                    let mut new_trait_item = trait_item.clone();
                    new_trait_item.generics = new_generics;
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Method(method) = item {
                            let mut new_method = method.clone();
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = return_type.as_ref() {
                                    if let Some(segment) = segments.iter().next() {
                                        if let Some(const_generics) = const_generics.iter().find(|param| {
                                            if let GenericParam::Const(param) = param {
                                                param.ident == segment.ident
                                            } else {
                                                false
                                            }
                                        }) {
                                            if let GenericParam::Const(param) = const_generics {
                                                let replacement = param.default.unwrap();
                                                let new_return_type = parse_quote! { #replacement };
                                                new_method.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                                            }
                                        }
                                    }
                                }
                            }
                            new_trait_item.items.push(syn::TraitItem::Method(new_method));
                        }
                    }
                    *item = Item::Trait(new_trait_item);
                }
            }
            if let Item::Impl(impl_item) = item {
                if let Some(const_generics) = impl_item.generics.params {
                    let mut new_generics = impl_item.generics.clone();
                    new_generics.params = Punctuated::new();
                    let mut new_impl_item = impl_item.clone();
                    new_impl_item.generics = new_generics;
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Method(method) = impl_item {
                            let mut new_method = method.clone();
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = return_type.as_ref() {
                                    if let Some(segment) = segments.iter().next() {
                                        if let Some(const_generics) = const_generics.iter().find(|param| {
                                            if let GenericParam::Const(param) = param {
                                                param.ident == segment.ident
                                            } else {
                                                false
                                            }
                                        }) {
                                            if let GenericParam::Const(param) = const_generics {
                                                let replacement = param.default.unwrap();
                                                let new_return_type = parse_quote! { #replacement };
                                                new_method.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                                            }
                                        }
                                    }
                                }
                            }
                            new_impl_item.items.push(syn::ImplItem::Method(new_method));
                        }
                    }
                    *item = Item::Impl(new_impl_item);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-generic const values in trait and impl definitions. It aims to test the compiler's handling of const generics and their interactions with trait and impl definitions."
    }
}