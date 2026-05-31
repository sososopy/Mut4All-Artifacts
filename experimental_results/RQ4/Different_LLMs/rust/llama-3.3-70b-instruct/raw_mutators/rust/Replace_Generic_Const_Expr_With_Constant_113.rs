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

pub struct Replace_Generic_Const_Expr_With_Constant_113;

impl Mutator for Replace_Generic_Const_Expr_With_Constant_113 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Expr_With_Constant_113"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::Where(clause) = &struct_item.generics {
                    for generic_param in &struct_item.generics.params {
                        if let syn::GenericParam::Const(const_param) = generic_param {
                            if let Some(default) = &const_param.default {
                                let constant_value = match const_param.ty {
                                    syn::Type::Path(ref path) => {
                                        if path.path.is_ident("usize") {
                                            quote!(0usize)
                                        } else if path.path.is_ident("isize") {
                                            quote!(0isize)
                                        } else if path.path.is_ident("bool") {
                                            quote!(false)
                                        } else if path.path.is_ident("char") {
                                            quote!('a')
                                        } else {
                                            default.clone()
                                        }
                                    }
                                    _ => default.clone(),
                                };
                                const_param.default = Some(constant_value);
                            }
                        }
                    }
                }
            } else if let syn::Item::Enum(enum_item) = item {
                if let syn::Generics::Where(clause) = &enum_item.generics {
                    for generic_param in &enum_item.generics.params {
                        if let syn::GenericParam::Const(const_param) = generic_param {
                            if let Some(default) = &const_param.default {
                                let constant_value = match const_param.ty {
                                    syn::Type::Path(ref path) => {
                                        if path.path.is_ident("usize") {
                                            quote!(0usize)
                                        } else if path.path.is_ident("isize") {
                                            quote!(0isize)
                                        } else if path.path.is_ident("bool") {
                                            quote!(false)
                                        } else if path.path.is_ident("char") {
                                            quote!('a')
                                        } else {
                                            default.clone()
                                        }
                                    }
                                    _ => default.clone(),
                                };
                                const_param.default = Some(constant_value);
                            }
                        }
                    }
                }
            } else if let syn::Item::Fn(func_item) = item {
                if let syn::Generics::Where(clause) = &func_item.sig.generics {
                    for generic_param in &func_item.sig.generics.params {
                        if let syn::GenericParam::Const(const_param) = generic_param {
                            if let Some(default) = &const_param.default {
                                let constant_value = match const_param.ty {
                                    syn::Type::Path(ref path) => {
                                        if path.path.is_ident("usize") {
                                            quote!(0usize)
                                        } else if path.path.is_ident("isize") {
                                            quote!(0isize)
                                        } else if path.path.is_ident("bool") {
                                            quote!(false)
                                        } else if path.path.is_ident("char") {
                                            quote!('a')
                                        } else {
                                            default.clone()
                                        }
                                    }
                                    _ => default.clone(),
                                };
                                const_param.default = Some(constant_value);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const expressions in structs, enums, and functions. It replaces the generic const expression with a constant value of the same type, testing the compiler's handling of constant values in generic const expressions and potentially exposing bugs related to constant folding and evaluation."
    }
}