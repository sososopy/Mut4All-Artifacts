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

pub struct Replace_Sized_Type_With_Unsized_Type_182;

impl Mutator for Replace_Sized_Type_With_Unsized_Type_182 {
    fn name(&self) -> &str {
        "Replace_Sized_Type_With_Unsized_Type_182"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(array_type) = &field.ty {
                        let element_type = &array_type.elem;
                        let unsized_type = Type::Slice(TypeSlice {
                            bracket_token: token::Bracket {
                                span: Span::call_site(),
                                delimiter: token::Delimiter::None,
                            },
                            elem: element_type.clone(),
                        });
                        field.ty = unsized_type;
                    } else if let Type::Path(type_path) = &field.ty {
                        if type_path.path.segments.last().unwrap().ident == "usize" {
                            let unsized_type = Type::Reference(TypeReference {
                                and_token: token::And::default(),
                                lifetime: None,
                                mutability: None,
                                elem: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("usize", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                })),
                            });
                            field.ty = unsized_type;
                        }
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for arg in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.last().unwrap().ident == "usize" {
                                let unsized_type = Type::Reference(TypeReference {
                                    and_token: token::And::default(),
                                    lifetime: None,
                                    mutability: None,
                                    elem: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("usize", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })),
                                });
                                pat_type.ty = Box::new(unsized_type);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces sized types with unsized types in struct fields and function arguments. This transformation aims to test the compiler's handling of type layouts and sizing, potentially revealing issues with the compiler's handling of type layouts and sizing."
    }
}

impl Replace_Sized_Type_With_Unsized_Type_182 {
    pub fn new() -> Self {
        Replace_Sized_Type_With_Unsized_Type_182
    }
}