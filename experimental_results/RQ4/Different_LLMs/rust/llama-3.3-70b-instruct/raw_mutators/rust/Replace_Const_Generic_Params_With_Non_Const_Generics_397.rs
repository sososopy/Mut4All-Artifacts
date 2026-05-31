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

pub struct Replace_Const_Generic_Params_With_Non_Const_Generics_397;

impl Mutator for Replace_Const_Generic_Params_With_Non_Const_Generics_397 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Params_With_Non_Const_Generics_397"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Const(param_const) = param {
                        let new_param = GenericParam::Type(GenericParamType {
                            attrs: param_const.attrs.clone(),
                            default: None,
                            ident: param_const.ident.clone(),
                            colon_token: param_const.colon_token.clone(),
                            bounds: param_const.bounds.clone(),
                            eq_token: None,
                            default_type: None,
                        });
                        *param = GenericParam::Type(new_param);
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param_const) = param {
                        let new_param = GenericParam::Type(GenericParamType {
                            attrs: param_const.attrs.clone(),
                            default: None,
                            ident: param_const.ident.clone(),
                            colon_token: param_const.colon_token.clone(),
                            bounds: param_const.bounds.clone(),
                            eq_token: None,
                            default_type: None,
                        });
                        *param = GenericParam::Type(new_param);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters in trait implementations and function definitions. This transformation removes the const constraint, potentially exposing bugs in the Rust compiler related to generic const expressions."
    }
}