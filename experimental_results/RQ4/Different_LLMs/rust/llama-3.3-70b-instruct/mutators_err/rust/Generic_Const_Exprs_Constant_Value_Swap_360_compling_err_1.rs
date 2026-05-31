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

pub struct Generic_Const_Exprs_Constant_Value_Swap_360;

impl Mutator for Generic_Const_Exprs_Constant_Value_Swap_360 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Constant_Value_Swap_360"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    let const_params: Vec<&mut GenericParam> = generics
                        .params
                        .iter_mut()
                        .filter(|param| {
                            if let GenericParam::Const(_) = param {
                                true
                            } else {
                                false
                            }
                        })
                        .collect();

                    if const_params.len() > 1 {
                        let mut rng = thread_rng();
                        const_params.shuffle(&mut rng);

                        let new_params: Vec<GenericParam> = const_params
                            .iter()
                            .map(|param| param.clone())
                            .collect();

                        generics.params = new_params;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the constant values of generic const expression parameters in function definitions. This transformation aims to test the compiler's handling of generic const expressions and may lead to ICEs or inference failures in the const evaluation system."
    }
}