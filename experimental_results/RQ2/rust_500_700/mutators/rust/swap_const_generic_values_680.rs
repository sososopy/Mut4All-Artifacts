use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng};
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Swap_Const_Generic_Values_680;

impl Mutator for Swap_Const_Generic_Values_680 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Values_680"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut const_values = Vec::new();

        // Collect all constant values from the generics
        for item in &file.items {
            match item {
                Item::Struct(ItemStruct { generics, .. }) | Item::Fn(ItemFn { sig: syn::Signature { generics, .. }, .. }) => {
                    for param in &generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &const_param.default {
                                const_values.push(default.clone());
                            }
                        }
                    }
                }
                _ => {}
            }
        }

        // If there are less than 2 const values, introduce a simple constant
        if const_values.len() < 2 {
            const_values.push(parse_quote!(0));
        }

        // Shuffle the collected constant values for swapping
        const_values.shuffle(&mut thread_rng());

        // Apply the swapped values back to the generics
        for item in &mut file.items {
            match item {
                Item::Struct(ItemStruct { generics, .. }) | Item::Fn(ItemFn { sig: syn::Signature { generics, .. }, .. }) => {
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &mut const_param.default {
                                if let Some(new_value) = const_values.pop() {
                                    *default = new_value;
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator swaps constant generic parameter values within the program. By doing so, it tests the compiler's handling of constant generics and their substitution, potentially uncovering issues related to constant evaluation and type compatibility."
    }
}