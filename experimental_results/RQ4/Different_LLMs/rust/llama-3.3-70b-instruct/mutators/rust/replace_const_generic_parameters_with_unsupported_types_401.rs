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

pub struct Replace_Const_Generic_Parameters_With_Unsupported_Types_401;

impl Mutator for Replace_Const_Generic_Parameters_With_Unsupported_Types_401 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameters_With_Unsupported_Types_401"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Type::Path(type_path) = &param.ty {
                            let new_type = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                                        ident: Ident::new("Dimension", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            });
                            param.ty = new_type;
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        for param in &mut generics.params {
                            if let GenericParam::Const(param) = param {
                                if let Type::Path(type_path) = &param.ty {
                                    let new_type = syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![syn::PathSegment {
                                                ident: Ident::new("Dimension", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    });
                                    param.ty = new_type;
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Struct(struct_item) = item {
                let generics = &mut struct_item.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Type::Path(type_path) = &param.ty {
                            let new_type = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                                        ident: Ident::new("Dimension", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            });
                            param.ty = new_type;
                        }
                    }
                }
            }

            if let syn::Item::Trait(trait_item) = item {
                let generics = &mut trait_item.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Type::Path(type_path) = &param.ty {
                            let new_type = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                                        ident: Ident::new("Dimension", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            });
                            param.ty = new_type;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with unsupported types, such as custom structs or enums, to test the compiler's handling of such cases. It applies to const generic parameters used in structs, functions, traits, and trait implementations."
    }
}