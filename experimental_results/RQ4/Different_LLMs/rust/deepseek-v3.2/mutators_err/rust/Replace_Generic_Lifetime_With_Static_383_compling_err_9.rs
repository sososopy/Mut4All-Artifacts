use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Lifetime as GenericParamLifetime, Generics, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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
use syn::meta::ParseNestedMeta;

use crate::mutator::Mutator;

pub struct Replace_Generic_Lifetime_With_Static_383;

impl Mutator for Replace_Generic_Lifetime_With_Static_383 {
    fn name(&self) -> &str {
        "Replace_Generic_Lifetime_With_Static_383"
    }
    fn mutate(&self, file: &mut syn::File) {
        let has_feature = file.items.iter().any(|item| {
            if let Item::Verbatim(tokens) = item {
                let tokens_str = tokens.to_string();
                if tokens_str.contains("#![feature(generic_const_exprs)]") {
                    return true;
                }
            }
            false
        });
        if !has_feature {
            return;
        }
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    let generics = &mut item_struct.generics;
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            if lifetime_param.lifetime.ident != "static" {
                                lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                                break;
                            }
                        }
                    }
                }
                Item::Impl(item_impl) => {
                    let generics = &mut item_impl.generics;
                    for param in &mut generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            if lifetime_param.lifetime.ident != "static" {
                                lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                                break;
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the first generic lifetime parameter in struct and impl definitions with the reserved lifetime 'static, but only when the generic_const_exprs feature is enabled. This transformation introduces a lifetime resolution conflict because 'static cannot be used as a generic parameter name. It aims to trigger internal compiler errors in the const generics or borrow-checking phases, especially when associated constants or generic const expressions are involved."
    }
}