use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
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

pub struct Change_Type_Annotation_186;

impl Mutator for Change_Type_Annotation_186 {
    fn name(&self) -> &str {
        "Change_Type_Annotation_186"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types = HashSet::new();
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                existing_types.insert(struct_item.ident.clone());
            }
            if let Item::Enum(enum_item) = item {
                existing_types.insert(enum_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Some(existing_type) = existing_types.iter().choose(&mut thread_rng()) {
                            pat_type.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: existing_type.clone(),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        }
                    }
                }
                if let ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                    if let Some(existing_type) = existing_types.iter().choose(&mut thread_rng()) {
                        *ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: existing_type.clone(),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        }));
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Some(existing_type) = existing_types.iter().choose(&mut thread_rng()) {
                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: existing_type.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }));
                                }
                            }
                        }
                        if let ReturnType::Type(_, ref mut ty) = &mut method.sig.output {
                            if let Some(existing_type) = existing_types.iter().choose(&mut thread_rng()) {
                                *ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: existing_type.clone(),
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the type annotations in the given code to a different type that is already defined in the program. This transformation tests the robustness of the Rust compiler in handling different types and their interactions."
    }
}