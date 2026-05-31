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

pub struct Alter_Const_Generic_Bounds_536;

impl Mutator for Alter_Const_Generic_Bounds_536 {
    fn name(&self) -> &str {
        "Alter_Const_Generic_Bounds_536"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if let Some(generic_param) = item_fn.sig.generics.params.iter_mut().find(|param| {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ident == "N"
                        } else {
                            false
                        }
                    }) {
                        if let GenericParam::Const(const_param) = generic_param {
                            const_param.default = Some(parse_quote!(2 + 1));
                        }
                    }
                }
                Item::Struct(item_struct) => {
                    if let Some(generic_param) = item_struct.generics.params.iter_mut().find(|param| {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ident == "N"
                        } else {
                            false
                        }
                    }) {
                        if let GenericParam::Const(const_param) = generic_param {
                            const_param.default = Some(parse_quote!(N * 1));
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}