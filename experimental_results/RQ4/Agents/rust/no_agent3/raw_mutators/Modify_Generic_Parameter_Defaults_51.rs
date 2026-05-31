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

pub struct Modify_Generic_Parameter_Defaults_51;

impl Mutator for Modify_Generic_Parameter_Defaults_51 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Defaults_51"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                let mut params_with_defaults = Vec::new();
                let mut other_params = Vec::new();

                for param in &generics.params {
                    match param {
                        GenericParam::Const(const_param) if const_param.default.is_some() => {
                            params_with_defaults.push(param.clone());
                        }
                        _ => {
                            other_params.push(param.clone());
                        }
                    }
                }

                if !params_with_defaults.is_empty() {
                    let mut new_params = other_params;
                    new_params.extend(params_with_defaults);

                    generics.params = Punctuated::from_iter(new_params);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions using const generics with default values. It ensures that any generic parameter with a default value is moved to the end of the list, maintaining compliance with Rust's requirement for trailing defaults. This can provoke the compiler into handling edge cases around parameter ordering."
    }
}