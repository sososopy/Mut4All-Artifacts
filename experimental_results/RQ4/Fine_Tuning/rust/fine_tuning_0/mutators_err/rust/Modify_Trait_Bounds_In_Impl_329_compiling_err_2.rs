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

pub struct Modify_Trait_Bounds_In_Impl_329;

impl Mutator for Modify_Trait_Bounds_In_Impl_329 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_In_Impl_329"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.defaultness.is_some() {
                    let generics = &item_impl.generics;
                    if let Some(first_param) = generics.params.first() {
                        if let syn::GenericParam::Type(type_param) = first_param {
                            let new_bound: syn::TypeParamBound = parse_quote!(Deref);
                            if !type_param.bounds.iter().any(|b| matches!(b, syn::TypeParamBound::Trait(tb) if tb.path.is_ident("Deref"))) {
                                let mut new_generics = generics.clone();
                                if let Some(syn::GenericParam::Type(ref mut type_param)) = new_generics.params.first_mut() {
                                    type_param.bounds.push(new_bound);
                                }
                                item_impl.generics = new_generics;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations marked with `const`. It adds a `T: Deref` constraint to the first type parameter if it doesn't already exist. This modification tests the compiler's handling of additional trait bounds in `const` contexts, potentially exposing bugs in trait resolution or const trait implementation logic."
    }
}