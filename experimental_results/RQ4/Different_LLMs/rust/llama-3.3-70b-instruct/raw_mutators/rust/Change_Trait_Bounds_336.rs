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

pub struct Change_Trait_Bounds_336;

impl Mutator for Change_Trait_Bounds_336 {
    fn name(&self) -> &str {
        "Change_Trait_Bounds_336"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(bound) = &*pat_type.ty {
                            if let Some(dyn_token) = bound.dyn_token {
                                // Replace dyn with a concrete type
                                let concrete_type = syn::Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(
                                                "ConcreteType",
                                                proc_macro2::Span::call_site(),
                                            ),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                });
                                pat_type.ty = Box::new(concrete_type);
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(bound) = &*pat_type.ty {
                                    if let Some(dyn_token) = bound.dyn_token {
                                        // Replace dyn with a concrete type
                                        let concrete_type = syn::Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new(
                                                        "ConcreteType",
                                                        proc_macro2::Span::call_site(),
                                                    ),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        });
                                        pat_type.ty = Box::new(concrete_type);
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
        "The mutation operator replaces the dyn keyword in trait bounds with a concrete type, testing the interaction between dynamic dispatch and trait bounds."
    }
}