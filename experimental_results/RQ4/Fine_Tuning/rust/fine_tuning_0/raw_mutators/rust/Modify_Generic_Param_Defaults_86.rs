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

pub struct Modify_Generic_Param_Defaults_86;

impl Mutator for Modify_Generic_Param_Defaults_86 {
    fn name(&self) -> &str {
        "Modify_Generic_Param_Defaults_86"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut params_with_defaults = vec![];
                let mut params_without_defaults = vec![];
                
                for param in &item_struct.generics.params {
                    match param {
                        GenericParam::Const(const_param) => {
                            if const_param.default.is_some() {
                                params_with_defaults.push(param.clone());
                            } else {
                                params_without_defaults.push(param.clone());
                            }
                        }
                        _ => params_without_defaults.push(param.clone()),
                    }
                }

                if !params_with_defaults.is_empty() {
                    item_struct.generics.params.clear();
                    item_struct.generics.params.extend(params_without_defaults);
                    item_struct.generics.params.extend(params_with_defaults);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generics that have default values. It ensures that any generic parameter with a default value is moved to the end of the list of generic parameters, complying with Rust's requirement that parameters with default values must be trailing. This transformation tests the compiler's handling of generic parameter ordering and default values."
    }
}