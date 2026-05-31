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

pub struct Remove_Lifetime_Parameters_445;

impl Mutator for Remove_Lifetime_Parameters_445 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameters_445"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    let generics_ref = generics;
                    generics_ref.params.retain(|param| {
                        match param {
                            GenericParam::Lifetime(_) => false,
                            _ => true,
                        }
                    });
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    let generics_ref = generics;
                    generics_ref.params.retain(|param| {
                        match param {
                            GenericParam::Lifetime(_) => false,
                            _ => true,
                        }
                    });
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(generics) = &mut func.sig.generics {
                            let generics_ref = generics;
                            generics_ref.params.retain(|param| {
                                match param {
                                    GenericParam::Lifetime(_) => false,
                                    _ => true,
                                }
                            });
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    let generics_ref = generics;
                    generics_ref.params.retain(|param| {
                        match param {
                            GenericParam::Lifetime(_) => false,
                            _ => true,
                        }
                    });
                }
            }
            if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    let generics_ref = generics;
                    generics_ref.params.retain(|param| {
                        match param {
                            GenericParam::Lifetime(_) => false,
                            _ => true,
                        }
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator Remove_Lifetime_Parameters_445 removes lifetime parameters from generic types and traits. This transformation tests the compiler's handling of lifetime and borrowing semantics by removing these constraints, potentially leading to errors or unexpected behavior."
    }
}