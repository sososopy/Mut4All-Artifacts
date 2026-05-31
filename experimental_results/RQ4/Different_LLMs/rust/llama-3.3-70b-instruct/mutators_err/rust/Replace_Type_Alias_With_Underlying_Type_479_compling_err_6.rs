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

pub struct Replace_Type_Alias_With_Underlying_Type_479;

impl Mutator for Replace_Type_Alias_With_Underlying_Type_479 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Underlying_Type_479"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments,
                    },
                }) = item_type.ty {
                    if let Some(segment) = segments.last() {
                        if let Some(underlying_type) = get_underlying_type(item_type) {
                            let mut file_clone = file.clone();
                            replace_type_aliases(&mut file_clone, segment.ident.clone(), underlying_type);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type aliases with their underlying types in the given code, specifically targeting type aliases defined using the `type` keyword and having an underlying type that is a trait object. This transformation helps expose bugs related to type alias handling and trait object typing in the Rust compiler."
    }
}

fn get_underlying_type(item_type: &syn::ItemType) -> Option<Box<syn::Type>> {
    if let Type::ImplTrait(TypeImplTrait {
        impl_token: _,
        bounds,
    }) = item_type.ty {
        if let Some(bound) = bounds.first() {
            if let TypeParamBound::Trait(TraitBound {
                paren_token: _,
                modifier: _,
                lifetimes: _,
                path: SynPath {
                    leading_colon: _,
                    segments,
                },
            }) = bound {
                if let Some(segment) = segments.last() {
                    if segment.ident == "Sized" {
                        return Some(Box::new(syn::Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds: punctuated::Punctuated::from_iter(vec![TypeParamBound::Trait(
                                TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: punctuated::Punctuated::from_iter(vec![
                                            PathSegment {
                                                ident: Ident::new("Sized", Span::call_site()),
                                                arguments: PathArguments::None,
                                            },
                                        ]),
                                    },
                                },
                            )]),
                        })));
                    }
                }
            }
        }
    }
    None
}

fn replace_type_aliases(file: &mut syn::File, alias: Ident, underlying_type: Box<syn::Type>) {
    for item in &mut file.items {
        if let syn::Item::Fn(func) = item {
            for input in &mut func.sig.inputs {
                if let FnArg::Typed(pat_type) = input {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                        },
                    }) = pat_type.ty {
                        if let Some(segment) = segments.last() {
                            if segment.ident == alias {
                                *pat_type.ty = *underlying_type.clone();
                            }
                        }
                    }
                }
            }
            if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments,
                    },
                }) = **return_type {
                    if let Some(segment) = segments.last() {
                        if segment.ident == alias {
                            *return_type = Box::new(*underlying_type.clone());
                        }
                    }
                }
            }
        }
        if let syn::Item::Impl(impl_item) = item {
            for impl_item in &mut impl_item.items {
                if let syn::ImplItem::Fn(method) = impl_item {
                    for input in &mut method.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = pat_type.ty {
                                if let Some(segment) = segments.last() {
                                    if segment.ident == alias {
                                        *pat_type.ty = *underlying_type.clone();
                                    }
                                }
                            }
                        }
                    }
                    if let ReturnType::Type(_, ref mut return_type) = method.sig.output {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = **return_type {
                            if let Some(segment) = segments.last() {
                                if segment.ident == alias {
                                    *return_type = Box::new(*underlying_type.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}