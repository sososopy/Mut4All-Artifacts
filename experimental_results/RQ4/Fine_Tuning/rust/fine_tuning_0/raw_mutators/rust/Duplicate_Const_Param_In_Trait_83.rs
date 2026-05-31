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

pub struct Duplicate_Const_Param_In_Trait_83;

impl Mutator for Duplicate_Const_Param_In_Trait_83 {
    fn name(&self) -> &str {
        "Duplicate_Const_Param_In_Trait_83"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut const_params = vec![];
                for param in &trait_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_params.push(const_param.clone());
                    }
                }

                if !const_params.is_empty() {
                    for const_param in const_params {
                        let duplicate_param: GenericParam = parse_quote!(const #const_param = #const_param.ident);
                        trait_item.generics.params.push(duplicate_param);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates const generic parameters in trait definitions. By introducing a duplicate with the same name and type, it aims to provoke compilation errors or ICEs, testing the compiler's handling of const generics and name resolution within trait contexts."
    }
}