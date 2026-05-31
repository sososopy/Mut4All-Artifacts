use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Generic_Size_Const_Param_402;

impl Mutator for Add_Generic_Size_Const_Param_402 {
    fn name(&self) -> &str {
        "Add_Generic_Size_Const_Param_402"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(first_type) = get_first_type_param(&item_struct.generics) {
                    let param_str = format!("const SIZE: usize = core::mem::size_of::<{}>();", first_type);
                    let new_param: GenericParam = syn::parse_str(&param_str).unwrap();
                    item_struct.generics.params.push(new_param);
                }
            } else if let syn::Item::Enum(item_enum) = item {
                if let Some(first_type) = get_first_type_param(&item_enum.generics) {
                    let param_str = format!("const SIZE: usize = core::mem::size_of::<{}>();", first_type);
                    let new_param: GenericParam = syn::parse_str(&param_str).unwrap();
                    item_enum.generics.params.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a generic const parameter to structs and enums with existing type parameters. The const value is derived from the size of the first type parameter using `core::mem::size_of`, which engages variance inference and tests the compiler's handling of const generics in relation to type parameter size dependencies. This can expose bugs in const evaluation or generic parameter inference."
    }
}

fn get_first_type_param(generics: &syn::Generics) -> Option<Ident> {
    for param in &generics.params {
        if let GenericParam::Type(type_param) = param {
            return Some(type_param.ident.clone());
        }
    }
    None
}