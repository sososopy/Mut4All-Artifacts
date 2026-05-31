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

pub struct Replace_Type_Annotation_With_Alternative_Type_388;

impl Mutator for Replace_Type_Annotation_With_Alternative_Type_388 {
    fn name(&self) -> &str {
        "Replace_Type_Annotation_With_Alternative_Type_388"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut defined_types = HashSet::new();
        for item in &file.items {
            if let Item::Struct(_) = item {
                if let Item::Struct(struct_item) = item {
                    defined_types.insert(struct_item.ident.to_string());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                        },
                    }) = &**ty
                    {
                        if let Some(segment) = segments.last() {
                            if let Some(alternative_type) = defined_types.iter().find(|t| *t != &segment.ident.to_string()) {
                                *ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(alternative_type, Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }));
                            }
                        }
                    }
                }

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = &*pat_type.ty {
                            if let Some(segment) = segments.last() {
                                if let Some(alternative_type) = defined_types.iter().find(|t| *t != &segment.ident.to_string()) {
                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new(alternative_type, Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }));
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = &**ty {
                                if let Some(segment) = segments.last() {
                                    if let Some(alternative_type) = defined_types.iter().find(|t| *t != &segment.ident.to_string()) {
                                        *ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new(alternative_type, Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        }));
                                    }
                                }
                            }
                        }

                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = &*pat_type.ty {
                                    if let Some(segment) = segments.last() {
                                        if let Some(alternative_type) = defined_types.iter().find(|t| *t != &segment.ident.to_string()) {
                                            pat_type.ty = Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new(alternative_type, Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type annotations with alternative types defined in the code. This transformation tests the compiler's ability to handle different types and ensures that it can correctly infer and check types even when they are replaced with other valid types."
    }
}