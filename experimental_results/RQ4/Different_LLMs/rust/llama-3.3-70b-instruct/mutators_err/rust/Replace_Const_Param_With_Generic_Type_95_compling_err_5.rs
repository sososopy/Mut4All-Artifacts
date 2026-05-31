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

pub struct Replace_Const_Param_With_Generic_Type_95;

impl Mutator for Replace_Const_Param_With_Generic_Type_95 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Generic_Type_95"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = syn::TypeParam {
                            attrs: param.attrs.clone(),
                            ident: param.ident.clone(),
                            colon_token: None,
                            bounds: Default::default(),
                            default: None,
                            eq_token: None,
                            default_type: None,
                        };
                        *param = GenericParam::Type(new_param);
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(param) = param {
                                let new_param = syn::TypeParam {
                                    attrs: param.attrs.clone(),
                                    ident: param.ident.clone(),
                                    colon_token: None,
                                    bounds: Default::default(),
                                    default: None,
                                    eq_token: None,
                                    default_type: None,
                                };
                                *param = GenericParam::Type(new_param);
                            }
                        }
                    }
                }
            }

            if let syn::Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = syn::TypeParam {
                            attrs: param.attrs.clone(),
                            ident: param.ident.clone(),
                            colon_token: None,
                            bounds: Default::default(),
                            default: None,
                            eq_token: None,
                            default_type: None,
                        };
                        *param = GenericParam::Type(new_param);
                    }
                }
            }

            if let syn::Item::Trait(trait_item) = item {
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = syn::TypeParam {
                            attrs: param.attrs.clone(),
                            ident: param.ident.clone(),
                            colon_token: None,
                            bounds: Default::default(),
                            default: None,
                            eq_token: None,
                            default_type: None,
                        };
                        *param = GenericParam::Type(new_param);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions or types with const parameters and replaces the const parameter with a generic type parameter. This transformation aims to test the compiler's handling of const parameters in generic contexts, especially when const parameters are used with associated const equality."
    }
}