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

pub struct Change_Const_Param_Default_Value_540;

impl Mutator for Change_Const_Param_Default_Value_540 {
    fn name(&self) -> &str {
        "Change_Const_Param_Default_Value_540"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let Some(generics) = &mut item_type.generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            let new_expr: Expr = parse_quote! { {#default * 2} };
                            *default = new_expr;
                            return Some(generics);
                        }
                    }
                    None
                }) {
                    // Apply mutation to the first const generic parameter with a default value.
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}