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

pub struct Const_Parameter_Misplacement_403;

impl Mutator for Const_Parameter_Misplacement_403 {
    fn name(&self) -> &str {
        "Const_Parameter_Misplacement_403"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let mut const_param_index = None;
                let mut type_param_index = None;

                for (i, param) in generics.params.iter().enumerate() {
                    if let syn::GenericParam::Const(_) = param {
                        const_param_index = Some(i);
                    } else if let syn::GenericParam::Type(_) = param {
                        type_param_index = Some(i);
                    }
                }

                if let (Some(c_idx), Some(t_idx)) = (const_param_index, type_param_index) {
                    let params_vec: Vec<_> = generics.params.clone().into_pairs().collect();
                    let mut new_params = Punctuated::new();
                    for (i, pair) in params_vec.into_iter().enumerate() {
                        if i == c_idx {
                            new_params.push_value(pair.into_value().clone());
                        } else if i == t_idx {
                            new_params.push_value(pair.into_value().clone());
                        } else {
                            new_params.push(pair);
                        }
                    }
                    generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with const generic parameters and swaps their positions with type parameters. This transformation challenges the Rust compiler's handling of const generics by altering expected parameter positions, potentially leading to type resolution errors or ICEs."
    }
}