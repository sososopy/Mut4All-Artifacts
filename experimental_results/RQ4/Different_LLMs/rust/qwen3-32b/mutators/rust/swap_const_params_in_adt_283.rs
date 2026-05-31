use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Swap_Const_Params_In_ADT_283;

impl Mutator for Swap_Const_Params_In_ADT_283 {
    fn name(&self) -> &str {
        "Swap_Const_Params_In_ADT_283"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                self.swap_const_params(&mut item_struct.generics);
            } else if let syn::Item::Union(item_union) = item {
                self.swap_const_params(&mut item_union.generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of two const parameters in ADT (struct or union) declarations while preserving their names. This tests the compiler's ability to correctly track parameter order in MIR and ensures that references to these parameters remain consistent, potentially exposing bugs in const generic handling."
    }
}

impl Swap_Const_Params_In_ADT_283 {
    fn swap_const_params(&self, generics: &mut syn::Generics) {
        let const_indices: Vec<usize> = generics
            .params
            .iter()
            .enumerate()
            .filter_map(|(i, param)| {
                if let GenericParam::Const(_) = param {
                    Some(i)
                } else {
                    None
                }
            })
            .collect();
        if const_indices.len() < 2 {
            return;
        }
        let i1 = const_indices[0];
        let i2 = const_indices[1];
        let mut params: Vec<_> = generics.params.clone().into_iter().collect();
        params.swap(i1, i2);
        generics.params = Punctuated::from_iter(params.into_iter());
    }
}