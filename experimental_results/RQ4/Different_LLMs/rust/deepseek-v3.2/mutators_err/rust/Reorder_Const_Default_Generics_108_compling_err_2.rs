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

pub struct Reorder_Const_Default_Generics_108;

impl Mutator for Reorder_Const_Default_Generics_108 {
    fn name(&self) -> &str {
        "Reorder_Const_Default_Generics_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(ItemStruct { generics, .. }) |
                Item::Enum(syn::ItemEnum { generics, .. }) |
                Item::Union(syn::ItemUnion { generics, .. }) |
                Item::Trait(syn::ItemTrait { generics, .. }) |
                Item::Impl(syn::ItemImpl { generics, .. }) => {
                    let params = &mut generics.params;
                    let mut i = 0;
                    while i < params.len() - 1 {
                        let first = &params[i];
                        let second = &params[i + 1];
                        let should_swap = match (first, second) {
                            (GenericParam::Type(type_param), GenericParam::Const(const_param)) => {
                                type_param.default.is_none() && const_param.default.is_some()
                            }
                            (GenericParam::Lifetime(lifetime_param), GenericParam::Const(const_param)) => {
                                lifetime_param.bounds.is_empty() && const_param.default.is_some()
                            }
                            _ => false,
                        };
                        if should_swap {
                            let mut temp = params.clone();
                            temp.swap(i, i + 1);
                            generics.params = temp;
                            break;
                        }
                        i += 1;
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator reorders generic parameters in struct, enum, union, trait, and impl definitions. It swaps a type or lifetime parameter without a default with a following const parameter that has a default, placing the const parameter first. This transformation aims to expose ordering bugs in the compiler's handling of defaulted const generics, particularly when default values are present. It preserves all bounds and constraints while altering parameter sequence."
    }
}