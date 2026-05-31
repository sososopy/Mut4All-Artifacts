use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::PathBuf as Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath, 
    PathArguments, PathSegment, Punctuated, ReturnType, Stmt, TraitBound, TraitBoundModifier, 
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Trait_Bounds_With_Associated_Type_Bounds_446;

impl Mutator for Replace_Trait_Bounds_With_Associated_Type_Bounds_446 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_With_Associated_Type_Bounds_446"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            for segment in &type_path.path.segments {
                                if let Some(args) = &segment.arguments {
                                    if let PathArguments::AngleBracketed(args) = args {
                                        for bound in &args.args {
                                            if let GenericArgument::Type(Type::Path(TypePath {
                                                path: SynPath { segments, .. },
                                                ..
                                            })) = bound {
                                                for segment in segments {
                                                    if let Some(trait_bounds) = get_trait_bounds(segment) {
                                                        for bound in trait_bounds {
                                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                                let associated_type_bound = get_associated_type_bound(&trait_bound);
                                                                pat_type.ty = Box::new(Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath {
                                                                        leading_colon: None,
                                                                        segments: Punctuated::from_iter(vec![
                                                                            PathSegment {
                                                                                ident: Ident::new(
                                                                                    "AssociatedTypeBound",
                                                                                    Span::call_site(),
                                                                                ),
                                                                                arguments: PathArguments::AngleBracketed(
                                                                                    AngleBracketedGenericArguments {
                                                                                        colon2_token: None,
                                                                                        lt_token: token::Lt::default(),
                                                                                        args: Punctuated::from_iter(vec![
                                                                                            GenericArgument::Type(
                                                                                                Type::Path(TypePath {
                                                                                                    qself: None,
                                                                                                    path: SynPath {
                                                                                                        leading_colon: None,
                                                                                                        segments: Punctuated::from_iter(vec![
                                                                                                            PathSegment {
                                                                                                                ident: Ident::new(
                                                                                                                    "SomeSpecificType",
                                                                                                                    Span::call_site(),
                                                                                                                ),
                                                                                                                arguments: PathArguments::None,
                                                                                                            },
                                                                                                        ]),
                                                                                                    },
                                                                                                }),
                                                                                            ),
                                                                                        ]),
                                                                                        gt_token: token::Gt::default(),
                                                                                    },
                                                                                ),
                                                                            },
                                                                        ]),
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
                    }
                }
            }
            else if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    for segment in &type_path.path.segments {
                                        if let Some(args) = &segment.arguments {
                                            if let PathArguments::AngleBracketed(args) = args {
                                                for bound in &args.args {
                                                    if let GenericArgument::Type(Type::Path(TypePath {
                                                        path: SynPath { segments, .. },
                                                        ..
                                                    })) = bound {
                                                        for segment in segments {
                                                            if let Some(trait_bounds) = get_trait_bounds(segment) {
                                                                for bound in trait_bounds {
                                                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                                                        let associated_type_bound = get_associated_type_bound(&trait_bound);
                                                                        pat_type.ty = Box::new(Type::Path(TypePath {
                                                                            qself: None,
                                                                            path: SynPath {
                                                                                leading_colon: None,
                                                                                segments: Punctuated::from_iter(vec![
                                                                                    PathSegment {
                                                                                        ident: Ident::new(
                                                                                            "AssociatedTypeBound",
                                                                                            Span::call_site(),
                                                                                        ),
                                                                                        arguments: PathArguments::AngleBracketed(
                                                                                            AngleBracketedGenericArguments {
                                                                                                colon2_token: None,
                                                                                                lt_token: token::Lt::default(),
                                                                                                args: Punctuated::from_iter(vec![
                                                                                                    GenericArgument::Type(
                                                                                                        Type::Path(TypePath {
                                                                                                            qself: None,
                                                                                                            path: SynPath {
                                                                                                                leading_colon: None,
                                                                                                                segments: Punctuated::from_iter(vec![
                                                                                                                    PathSegment {
                                                                                                                        ident: Ident::new(
                                                                                                                            "SomeSpecificType",
                                                                                                                            Span::call_site(),
                                                                                                                        ),
                                                                                                                        arguments: PathArguments::None,
                                                                                                                    },
                                                                                                                ]),
                                                                                                            },
                                                                                                        }),
                                                                                                    ),
                                                                                                ]),
                                                                                                gt_token: token::Gt::default(),
                                                                                            },
                                                                                        ),
                                                                                    },
                                                                                ]),
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
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with associated type bounds in function and method parameters, introducing complex trait relationships and associated types to stress the compiler's ability to handle these features."
    }
}

fn get_trait_bounds(segment: &PathSegment) -> Option<&Punctuated<TypeParamBound, token::Comma>> {
    if let Some(args) = &segment.arguments {
        if let PathArguments::AngleBracketed(args) = args {
            return Some(&args.args);
        }
    }
    None
}

fn get_associated_type_bound(trait_bound: &TraitBound) -> TypeParamBound {
    TypeParamBound::Trait(TraitBound {
        paren_token: None,
        modifier: TraitBoundModifier::None,
        lifetimes: None,
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![PathSegment {
                ident: Ident::new("SomeSpecificType", Span::call_site()),
                arguments: PathArguments::None,
            }]),
        },
    })
}