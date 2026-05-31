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

pub struct Replace_Const_Generic_With_Non_Const_Version_351;

impl Mutator for Replace_Const_Generic_With_Non_Const_Version_351 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Const_Version_351"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: param.colon_token,
                                bounds: param.bounds.clone(),
                                default: None,
                                eq_token: None,
                                default_span: param.default_span,
                            });
                            *param = GenericParam::Type(new_param);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_param = GenericParam::Type(syn::TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: param.colon_token,
                                bounds: param.bounds.clone(),
                                default: None,
                                eq_token: None,
                                default_span: param.default_span,
                            });
                            *param = GenericParam::Type(new_param);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters. This transformation aims to test the compiler's handling of const generics and generic const expressions by replacing them with non-const versions, potentially triggering type resolution issues, const evaluation bugs, or unexpected behavior in generic function contexts."
    }
}