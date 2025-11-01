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

pub struct Swap_Const_Generics_73;

impl Mutator for Swap_Const_Generics_73 {
    fn name(&self) -> &str {
        "Swap_Const_Generics_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                let mut const_generic_with_default = None;
                let mut other_params = vec![];

                for param in generics.params.iter() {
                    match param {
                        GenericParam::Const(c) if c.default.is_some() => {
                            const_generic_with_default = Some(c.clone());
                        }
                        _ => {
                            other_params.push(param.clone());
                        }
                    }
                }

                if let Some(const_generic) = const_generic_with_default {
                    let mut new_params = Punctuated::new();
                    for param in other_params {
                        new_params.push(param);
                    }
                    new_params.push(GenericParam::Const(const_generic));
                    generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions with const generics. It identifies structs with at least one const generic parameter having a default value and another parameter. The operator swaps the order of these parameters, placing the const generic parameter with a default value after other parameters. This reordering can expose potential issues with const generics handling by the Rust compiler."
    }
}