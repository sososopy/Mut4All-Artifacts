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

pub struct Change_Closure_Argument_Type_491;

impl Mutator for Change_Closure_Argument_Type_491 {
    fn name(&self) -> &str {
        "Change_Closure_Argument_Type_491"
    }

    fn mutate(&self, file: &mut syn::File) {
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
                        }) = &*pat_type.ty
                        {
                            if let Some(segment) = segments.first() {
                                if segment.ident == "Box" {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                                        if let Some(generic_argument) = angle_bracketed.args.first() {
                                            if let GenericArgument::Type(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: inner_segments,
                                                },
                                            })) = generic_argument {
                                                if let Some(inner_segment) = inner_segments.first() {
                                                    if inner_segment.ident == "dyn" {
                                                        if let PathArguments::AngleBracketed(inner_angle_bracketed) = &inner_segment.arguments {
                                                            if let Some(inner_generic_argument) = inner_angle_bracketed.args.first() {
                                                                if let GenericArgument::Type(Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: inner_inner_segments,
                                                                })) = inner_generic_argument {
                                                                    if let Some(inner_inner_segment) = inner_inner_segments.segments.first() {
                                                                        if inner_inner_segment.ident == "Fn" {
                                                                            let mut rng = thread_rng();
                                                                            let closure_types = ["Fn", "FnMut", "FnOnce"];
                                                                            let new_closure_type = closure_types.choose(&mut rng).unwrap();
                                                                            // We cannot directly mutate the Ident here because it's behind a reference.
                                                                            // We need to clone the entire TypePath and replace the Ident in the cloned version.
                                                                            let new_inner_inner_segment = Ident::new(new_closure_type, Span::call_site());
                                                                            let new_inner_inner_segments = syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                                                                ident: new_inner_inner_segment,
                                                                                arguments: inner_inner_segment.arguments.clone(),
                                                                            }]);
                                                                            let new_inner_generic_argument = GenericArgument::Type(Type::Path(TypePath {
                                                                                qself: None,
                                                                                path: SynPath {
                                                                                    leading_colon: None,
                                                                                    segments: new_inner_inner_segments,
                                                                                },
                                                                            }));
                                                                            let new_inner_angle_bracketed = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                                colon2_token: inner_angle_bracketed.colon2_token,
                                                                                lt_token: inner_angle_bracketed.lt_token,
                                                                                args: syn::punctuated::Punctuated::from_iter(vec![new_inner_generic_argument]),
                                                                                gt_token: inner_angle_bracketed.gt_token,
                                                                            });
                                                                            let new_inner_segment = syn::PathSegment {
                                                                                ident: inner_segment.ident.clone(),
                                                                                arguments: PathArguments::AngleBracketed(new_inner_angle_bracketed),
                                                                            };
                                                                            let new_inner_segments = syn::punctuated::Punctuated::from_iter(vec![new_inner_segment]);
                                                                            let new_generic_argument = GenericArgument::Type(Type::Path(TypePath {
                                                                                qself: None,
                                                                                path: SynPath {
                                                                                    leading_colon: None,
                                                                                    segments: new_inner_segments,
                                                                                },
                                                                            }));
                                                                            let new_angle_bracketed = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                                colon2_token: angle_bracketed.colon2_token,
                                                                                lt_token: angle_bracketed.lt_token,
                                                                                args: syn::punctuated::Punctuated::from_iter(vec![new_generic_argument]),
                                                                                gt_token: angle_bracketed.gt_token,
                                                                            });
                                                                            let new_segment = syn::PathSegment {
                                                                                ident: segment.ident.clone(),
                                                                                arguments: PathArguments::AngleBracketed(new_angle_bracketed),
                                                                            };
                                                                            let new_segments = syn::punctuated::Punctuated::from_iter(vec![new_segment]);
                                                                            pat_type.ty = Type::Path(TypePath {
                                                                                qself: None,
                                                                                path: SynPath {
                                                                                    leading_colon: None,
                                                                                    segments: new_segments,
                                                                                },
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions that take closures as arguments and changes the type of the closure argument. It randomly selects a new closure type (Fn, FnMut, or FnOnce) and replaces the existing type with the new one. This transformation aims to test the compiler's handling of different closure types and their interactions with the rest of the code."
    }
}