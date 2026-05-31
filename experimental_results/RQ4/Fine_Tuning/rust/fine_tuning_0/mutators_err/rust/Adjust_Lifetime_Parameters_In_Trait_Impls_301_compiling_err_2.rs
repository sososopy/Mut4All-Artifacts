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
    TraitItemMethod,
    TraitItem,
    ImplItem,
};

use crate::mutator::Mutator;

pub struct Adjust_Lifetime_Parameters_In_Trait_Impls_301;

impl Mutator for Adjust_Lifetime_Parameters_In_Trait_Impls_301 {
    fn name(&self) -> &str {
        "Adjust_Lifetime_Parameters_In_Trait_Impls_301"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods = HashSet::new();

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &item_trait.items {
                    if let TraitItem::Method(trait_method) = trait_item {
                        if trait_method.sig.generics.lifetimes().count() > 0 {
                            trait_methods.insert(trait_method.sig.ident.to_string());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(impl_method) = impl_item {
                            if trait_methods.contains(&impl_method.sig.ident.to_string()) {
                                impl_method.sig.generics.params = Punctuated::new();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies methods in traits with lifetime parameters and removes those parameters in the corresponding implementation methods. This creates a mismatch between the trait definition and its implementation, potentially triggering compiler errors related to lifetime management and substitution."
    }
}