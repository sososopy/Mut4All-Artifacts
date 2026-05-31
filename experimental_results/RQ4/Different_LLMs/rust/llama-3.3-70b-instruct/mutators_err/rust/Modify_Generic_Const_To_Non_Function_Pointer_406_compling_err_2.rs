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

pub struct Modify_Generic_Const_To_Non_Function_Pointer_406;

impl Mutator for Modify_Generic_Const_To_Non_Function_Pointer_406 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_To_Non_Function_Pointer_406"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(generics) = &struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Type::Path(type_path) = param.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "fn" {
                                        param.ty = Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("u32", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Enum(enum_item) = item {
                if let Some(generics) = &enum_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Type::Path(type_path) = param.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "fn" {
                                        param.ty = Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("u32", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs and enums with generic const parameters. It replaces the type of the generic const parameter from a function pointer to a non-function pointer type, such as u32. This transformation aims to test the compiler's handling of generic const parameters and function pointers, potentially triggering bugs in the Rust compiler."
    }
}