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

pub struct Swap_Const_Generic_Order_108;

impl Mutator for Swap_Const_Generic_Order_108 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Order_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics.params;
                let mut const_generic_with_default = None;
                let mut other_params = Vec::new();

                for param in generics.iter() {
                    match param {
                        syn::GenericParam::Const(const_param) => {
                            if const_param.default.is_some() {
                                const_generic_with_default = Some(param.clone());
                            } else {
                                other_params.push(param.clone());
                            }
                        }
                        _ => other_params.push(param.clone()),
                    }
                }

                if let Some(const_param) = const_generic_with_default {
                    other_params.push(const_param);
                    item_struct.generics.params = Punctuated::from_iter(other_params);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies structs with const generic parameters that have default values and swaps their order with other parameters. The goal is to test compiler behavior when const generics with defaults are reordered, potentially exposing issues with const generic handling and default value resolution."
    }
}