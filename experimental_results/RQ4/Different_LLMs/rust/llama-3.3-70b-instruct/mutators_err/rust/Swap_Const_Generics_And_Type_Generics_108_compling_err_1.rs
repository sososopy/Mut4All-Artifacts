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

pub struct Swap_Const_Generics_And_Type_Generics_108;

impl Mutator for Swap_Const_Generics_And_Type_Generics_108 {
    fn name(&self) -> &str {
        "Swap_Const_Generics_And_Type_Generics_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    let mut const_generics = Vec::new();
                    let mut type_generics = Vec::new();
                    for param in &mut generics.params {
                        match param {
                            GenericParam::Const(_) => const_generics.push(param.clone()),
                            GenericParam::Type(_) => type_generics.push(param.clone()),
                            _ => {}
                        }
                    }
                    generics.params.clear();
                    generics.params.extend(type_generics);
                    generics.params.extend(const_generics);
                }
            }
            if let syn::Item::Enum(enum_item) = item {
                if let Some(generics) = &mut enum_item.generics {
                    let mut const_generics = Vec::new();
                    let mut type_generics = Vec::new();
                    for param in &mut generics.params {
                        match param {
                            GenericParam::Const(_) => const_generics.push(param.clone()),
                            GenericParam::Type(_) => type_generics.push(param.clone()),
                            _ => {}
                        }
                    }
                    generics.params.clear();
                    generics.params.extend(type_generics);
                    generics.params.extend(const_generics);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of const generic parameters and type generic parameters in a generic struct or enum definition. This transformation tests the compiler's ability to handle the scoping and evaluation of const expressions in the context of generic definitions."
    }
}