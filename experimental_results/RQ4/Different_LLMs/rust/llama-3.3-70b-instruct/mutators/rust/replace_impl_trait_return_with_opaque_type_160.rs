use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Impl_Trait_Return_With_Opaque_Type_160;

impl Mutator for Replace_Impl_Trait_Return_With_Opaque_Type_160 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_Return_With_Opaque_Type_160"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let opaque_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(
                                TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: Ident::new("Sized", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                },
                            )]),
                        });
                        item_fn.sig.output = ReturnType::Type(
                            Default::default(),
                            Box::new(opaque_type),
                        );
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let opaque_type = Type::ImplTrait(TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(
                                        TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                    ident: Ident::new("Sized", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        },
                                    )]),
                                });
                                func.sig.output = ReturnType::Type(
                                    Default::default(),
                                    Box::new(opaque_type),
                                );
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that return impl trait types. It replaces the impl trait return type with an opaque type, which can trigger bugs related to trait resolution and type inference."
    }
}