use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Add_Const_Params_For_Generic_Types_94;

impl Mutator for Add_Const_Params_For_Generic_Types_94 {
    fn name(&self) -> &str {
        "Add_Const_Params_For_Generic_Types_94"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_params = Punctuated::new();
                for param in &func.sig.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: #type_param.ident);
                        new_params.push(new_const);
                    }
                }
                func.sig.generics.params = new_params;
            } else if let syn::Item::Struct(struct_item) = item {
                let mut new_params = Punctuated::new();
                for param in &struct_item.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: #type_param.ident);
                        new_params.push(new_const);
                    }
                }
                struct_item.generics.params = new_params;
            } else if let syn::Item::Trait(trait_item) = item {
                let mut new_params = Punctuated::new();
                for param in &trait_item.generics.params {
                    new_params.push(param.clone());
                    if let syn::GenericParam::Type(type_param) = param {
                        let const_name = format_ident!("C_{}", type_param.ident);
                        let new_const = parse_quote!(const #const_name: #type_param.ident);
                        new_params.push(new_const);
                    }
                }
                trait_item.generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}