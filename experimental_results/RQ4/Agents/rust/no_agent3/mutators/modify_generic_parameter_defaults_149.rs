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

pub struct Modify_Generic_Parameter_Defaults_149;

impl Mutator for Modify_Generic_Parameter_Defaults_149 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Defaults_149"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                let mut default_params = Vec::new();
                let mut non_default_params = Vec::new();

                for param in &generics.params {
                    match param {
                        GenericParam::Const(const_param) if const_param.default.is_some() => {
                            default_params.push(param.clone());
                        }
                        _ => non_default_params.push(param.clone()),
                    }
                }

                if !default_params.is_empty() && !non_default_params.is_empty() {
                    let mut new_params = non_default_params.clone();
                    new_params.append(&mut default_params);
                    generics.params = Punctuated::from_iter(new_params);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies structs with generic parameters that have default values and modifies the order of these parameters. The goal is to ensure that parameters with default values are not trailing, which could lead to unexpected behavior or errors during compilation. By reordering the parameters, this mutator tests the robustness of code that relies on specific generic parameter ordering."
    }
}