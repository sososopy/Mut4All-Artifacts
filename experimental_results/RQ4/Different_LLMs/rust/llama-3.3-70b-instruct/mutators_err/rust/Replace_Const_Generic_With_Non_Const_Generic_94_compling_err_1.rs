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

pub struct Replace_Const_Generic_With_Non_Const_Generic_94;

impl Mutator for Replace_Const_Generic_With_Non_Const_Generic_94 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Const_Generic_94"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut new_params = Punctuated::new();
                for param in trait_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = GenericParam::Type(GenericParamType {
                            attrs: param.attrs.clone(),
                            default: None,
                            ident: Ident::new(&format!("U{}", thread_rng().gen::<u32>()), param.ident.span()),
                            colon_token: param.colon_token,
                            bounds: param.bounds.clone(),
                        });
                        new_params.push(new_param);
                    } else {
                        new_params.push(param);
                    }
                }
                trait_item.generics.params = new_params;
            }
            if let syn::Item::Impl(impl_item) = item {
                let mut new_params = Punctuated::new();
                for param in impl_item.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = GenericParam::Type(GenericParamType {
                            attrs: param.attrs.clone(),
                            default: None,
                            ident: Ident::new(&format!("U{}", thread_rng().gen::<u32>()), param.ident.span()),
                            colon_token: param.colon_token,
                            bounds: param.bounds.clone(),
                        });
                        new_params.push(new_param);
                    } else {
                        new_params.push(param);
                    }
                }
                impl_item.generics.params = new_params;
            }
            if let syn::Item::Fn(func) = item {
                let mut new_params = Punctuated::new();
                for param in func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        let new_param = GenericParam::Type(GenericParamType {
                            attrs: param.attrs.clone(),
                            default: None,
                            ident: Ident::new(&format!("U{}", thread_rng().gen::<u32>()), param.ident.span()),
                            colon_token: param.colon_token,
                            bounds: param.bounds.clone(),
                        });
                        new_params.push(new_param);
                    } else {
                        new_params.push(param);
                    }
                }
                func.sig.generics.params = new_params;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets Rust code that uses const generics, specifically in the context of traits and implementations. It replaces a const generic parameter with a non-const generic parameter in a trait or implementation definition, preserving existing constraints and bounds on the original type parameters."
    }
}