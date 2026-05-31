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

pub struct Swap_Associated_Type_Definition_260;

impl Mutator for Swap_Associated_Type_Definition_260 {
    fn name(&self) -> &str {
        "Swap_Associated_Type_Definition_260"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut associated_types: Vec<(Ident, (Token![=], Type))> = Vec::new();
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let syn::TraitItem::Type(associated_type) = item {
                        if let Some((eq_token, ty)) = &associated_type.default {
                            associated_types.push((associated_type.ident.clone(), (eq_token.clone(), ty.clone())));
                        }
                    }
                }
            }
        }

        if associated_types.len() >= 2 {
            let mut rng = thread_rng();
            let idx1 = rng.gen_range(0..associated_types.len());
            let idx2 = rng.gen_range(0..associated_types.len());
            if idx1 != idx2 {
                let (ident1, (eq_token1, ty1)) = associated_types[idx1].clone();
                let (ident2, (eq_token2, ty2)) = associated_types[idx2].clone();
                for item in &mut file.items {
                    if let syn::Item::Trait(trait_item) = item {
                        for item in &mut trait_item.items {
                            if let syn::TraitItem::Type(associated_type) = item {
                                if associated_type.ident == ident1 {
                                    associated_type.default = Some((eq_token2.clone(), ty2.clone()));
                                } else if associated_type.ident == ident2 {
                                    associated_type.default = Some((eq_token1.clone(), ty1.clone()));
                                }
                            }
                        }
                    }
                    if let syn::Item::Impl(impl_item) = item {
                        for impl_item in &mut impl_item.items {
                            if let syn::ImplItem::Type(associated_type) = impl_item {
                                if let Some((eq_token, ty)) = &mut associated_type.default {
                                    if associated_type.ident == ident1 {
                                        *eq_token = eq_token2.clone();
                                        *ty = ty2.clone();
                                    } else if associated_type.ident == ident2 {
                                        *eq_token = eq_token1.clone();
                                        *ty = ty1.clone();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the definitions of two associated types in a trait. This transformation can help expose bugs related to associated type handling in the Rust compiler, such as incorrect type checking or code generation."
    }
}