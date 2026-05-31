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

pub struct Replace_Type_Parameter_With_Concrete_Type_185;

impl Mutator for Replace_Type_Parameter_With_Concrete_Type_185 {
    fn name(&self) -> &str {
        "Replace_Type_Parameter_With_Concrete_Type_185"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = vec![];
        for item in &file.items {
            if let Item::Struct(struct_def) = item {
                concrete_types.push(Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![PathSegment {
                            ident: struct_def.ident.clone(),
                            arguments: PathArguments::None,
                        }]),
                    },
                }));
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for param in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = &*pat_type.ty
                        {
                            if segments.len() == 1 && segments[0].ident.to_string().starts_with("T") {
                                if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                    pat_type.ty = Box::new(concrete_type.clone());
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_def) = item {
                for impl_item in &mut impl_def.items {
                    if let ImplItem::Method(func) = impl_item {
                        for param in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = param {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = &*pat_type.ty
                                {
                                    if segments.len() == 1 && segments[0].ident.to_string().starts_with("T") {
                                        if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                            pat_type.ty = Box::new(concrete_type.clone());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Trait(trait_def) = item {
                for item in &mut trait_def.items {
                    if let TraitItem::Method(method) = item {
                        for param in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = param {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = &*pat_type.ty
                                {
                                    if segments.len() == 1 && segments[0].ident.to_string().starts_with("T") {
                                        if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                            pat_type.ty = Box::new(concrete_type.clone());
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
        "The mutation operator replaces type parameters with concrete types in function and method signatures, as well as in trait definitions. This transformation aims to test the compiler's handling of type parameters and concrete types, potentially leading to type mismatches or inference failures."
    }
}