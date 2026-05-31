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

pub struct Modify_Generic_Parameter_Order_348;

impl Mutator for Modify_Generic_Parameter_Order_348 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Order_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let mut default_params = Vec::new();
                let mut non_default_params = Vec::new();

                for param in generics.params.iter() {
                    match param {
                        syn::GenericParam::Const(c) if c.default.is_some() => {
                            default_params.push(param.clone());
                        }
                        _ => non_default_params.push(param.clone()),
                    }
                }

                if !default_params.is_empty() && !non_default_params.is_empty() {
                    generics.params.clear();
                    generics.params.extend(non_default_params);
                    generics.params.extend(default_params);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with generic parameters, specifically rearranging them so that parameters with default values are not trailing. This transformation tests the compiler's handling of generic parameter ordering and default values, potentially exposing issues in type resolution or default value application."
    }
}