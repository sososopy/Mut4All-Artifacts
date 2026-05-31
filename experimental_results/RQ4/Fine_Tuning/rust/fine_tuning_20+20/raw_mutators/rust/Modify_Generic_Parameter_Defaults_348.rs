use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Generic_Parameter_Defaults_348;

impl Mutator for Modify_Generic_Parameter_Defaults_348 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Defaults_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let has_default = item_struct.generics.params.iter().any(|param| match param {
                    GenericParam::Const(const_param) => const_param.default.is_some(),
                    _ => false,
                });
                if !has_default {
                    continue;
                }
                let mut new_params = Punctuated::new();
                let mut default_params = Vec::new();
                for param in item_struct.generics.params.iter() {
                    match param {
                        GenericParam::Const(const_param) => {
                            if const_param.default.is_some() {
                                default_params.push(param.clone());
                            } else {
                                new_params.push(param.clone());
                            }
                        }
                        _ => new_params.push(param.clone()),
                    }
                }
                if !default_params.is_empty() {
                    new_params.extend(default_params);
                    item_struct.generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with generic parameters that have default values. It reorders the generic parameters to ensure that those with defaults are not trailing, thereby altering the expected parameter order and potentially exposing compiler bugs related to generic parameter parsing and resolution."
    }
}