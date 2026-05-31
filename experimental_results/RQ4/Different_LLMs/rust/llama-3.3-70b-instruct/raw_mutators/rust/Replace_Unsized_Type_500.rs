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

pub struct Replace_Unsized_Type_500;

impl Mutator for Replace_Unsized_Type_500 {
    fn name(&self) -> &str {
        "Replace_Unsized_Type_500"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        let new_type = if thread_rng().gen_bool(0.5) {
                            // Replace with a sized type
                            Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("i32", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }))
                        } else {
                            // Replace with an unsized type
                            Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("str", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }))
                        };
                        pat_type.ty = new_type;
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                let new_type = if thread_rng().gen_bool(0.5) {
                                    // Replace with a sized type
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("i32", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }))
                                } else {
                                    // Replace with an unsized type
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("str", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }))
                                };
                                pat_type.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces sized types with unsized types or vice versa in function and trait definitions. This transformation aims to trigger bugs related to sized and unsized types, such as type checking and inference issues."
    }
}