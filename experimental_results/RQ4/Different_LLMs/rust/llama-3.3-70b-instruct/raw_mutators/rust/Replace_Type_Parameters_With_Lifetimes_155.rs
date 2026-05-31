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

pub struct Replace_Type_Parameters_With_Lifetimes_155;

impl Mutator for Replace_Type_Parameters_With_Lifetimes_155 {
    fn name(&self) -> &str {
        "Replace_Type_Parameters_With_Lifetimes_155"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.replace_type_params_with_lifetimes_in_fn(func);
            } else if let syn::Item::Struct(struct_) = item {
                self.replace_type_params_with_lifetimes_in_struct(struct_);
            } else if let syn::Item::Enum(enum_) = item {
                self.replace_type_params_with_lifetimes_in_enum(enum_);
            } else if let syn::Item::Impl(impl_) = item {
                self.replace_type_params_with_lifetimes_in_impl(impl_);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type parameters with lifetime parameters in function signatures, struct definitions, and enum definitions. This transformation aims to increase the likelihood of triggering bugs related to variance and lifetime issues."
    }
}

impl Replace_Type_Parameters_With_Lifetimes_155 {
    fn replace_type_params_with_lifetimes_in_fn(&self, func: &mut syn::ItemFn) {
        for param in &mut func.sig.generics.params {
            if let GenericParam::Type(type_param) = param {
                *param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                    attrs: type_param.attrs.clone(),
                    lifetime: syn::Lifetime {
                        apostrophe: type_param.ident.span(),
                        ident: type_param.ident.clone(),
                    },
                    colon_token: type_param.colon_token.clone(),
                    bounds: type_param.bounds.clone(),
                });
            }
        }
    }

    fn replace_type_params_with_lifetimes_in_struct(&self, struct_: &mut syn::ItemStruct) {
        for param in &mut struct_.generics.params {
            if let GenericParam::Type(type_param) = param {
                *param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                    attrs: type_param.attrs.clone(),
                    lifetime: syn::Lifetime {
                        apostrophe: type_param.ident.span(),
                        ident: type_param.ident.clone(),
                    },
                    colon_token: type_param.colon_token.clone(),
                    bounds: type_param.bounds.clone(),
                });
            }
        }
    }

    fn replace_type_params_with_lifetimes_in_enum(&self, enum_: &mut syn::ItemEnum) {
        for param in &mut enum_.generics.params {
            if let GenericParam::Type(type_param) = param {
                *param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                    attrs: type_param.attrs.clone(),
                    lifetime: syn::Lifetime {
                        apostrophe: type_param.ident.span(),
                        ident: type_param.ident.clone(),
                    },
                    colon_token: type_param.colon_token.clone(),
                    bounds: type_param.bounds.clone(),
                });
            }
        }
    }

    fn replace_type_params_with_lifetimes_in_impl(&self, impl_: &mut syn::ItemImpl) {
        for param in &mut impl_.generics.params {
            if let GenericParam::Type(type_param) = param {
                *param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                    attrs: type_param.attrs.clone(),
                    lifetime: syn::Lifetime {
                        apostrophe: type_param.ident.span(),
                        ident: type_param.ident.clone(),
                    },
                    colon_token: type_param.colon_token.clone(),
                    bounds: type_param.bounds.clone(),
                });
            }
        }
    }
}