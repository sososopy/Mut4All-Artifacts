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

pub struct Swap_Const_Type_Generics_197;

impl Mutator for Swap_Const_Type_Generics_197 {
    fn name(&self) -> &str {
        "Swap_Const_Type_Generics_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some((const_generic, type_generic)) = find_const_and_type_generics(&item_struct.generics) {
                    let const_generic_ident = const_generic.ident.clone();
                    let type_generic_ident = type_generic.ident.clone();
                    
                    let default_expr: Expr = parse_quote!({1 + 1});
                    
                    let new_generics: Punctuated<GenericParam, Comma> = item_struct.generics.params.iter().map(|param| {
                        match param {
                            GenericParam::Type(type_param) if type_param.ident == type_generic_ident => {
                                GenericParam::Const(const_generic.clone())
                            },
                            GenericParam::Const(const_param) if const_param.ident == const_generic_ident => {
                                let mut new_const_param = const_param.clone();
                                new_const_param.default = Some(default_expr.clone());
                                GenericParam::Type(type_generic.clone())
                            },
                            _ => param.clone(),
                        }
                    }).collect();
                    
                    item_struct.generics.params = new_generics;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions with both const and type generics. It swaps the order of these generics and assigns a default value to the const generic if it doesn't have one. This transformation is intended to explore potential issues in the Rust compiler's handling of generics, especially when their order and default values are modified."
    }
}

fn find_const_and_type_generics<'a>(generics: &'a syn::Generics) -> Option<(&'a syn::ConstParam, &'a syn::TypeParam)> {
    let mut const_generic = None;
    let mut type_generic = None;
    
    for param in &generics.params {
        match param {
            GenericParam::Const(const_param) => {
                const_generic = Some(const_param);
            },
            GenericParam::Type(type_param) => {
                type_generic = Some(type_param);
            },
            _ => {}
        }
        
        if const_generic.is_some() && type_generic.is_some() {
            return Some((const_generic.unwrap(), type_generic.unwrap()));
        }
    }
    
    None
}