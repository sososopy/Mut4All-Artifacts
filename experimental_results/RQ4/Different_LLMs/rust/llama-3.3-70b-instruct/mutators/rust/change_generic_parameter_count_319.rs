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

pub struct Change_Generic_Parameter_Count_319;

impl Mutator for Change_Generic_Parameter_Count_319 {
    fn name(&self) -> &str {
        "Change_Generic_Parameter_Count_319"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                let generics = &mut func.sig.generics;
                if rng.gen_bool(0.5) {
                    // Add a new generic parameter
                    generics.params.push(parse_quote!(const N: usize));
                } else {
                    // Remove the last generic parameter if it exists
                    if !generics.params.is_empty() {
                        generics.params.pop();
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut rng = thread_rng();
                        let generics = &mut func.sig.generics;
                        if rng.gen_bool(0.5) {
                            // Add a new generic parameter
                            generics.params.push(parse_quote!(const N: usize));
                        } else {
                            // Remove the last generic parameter if it exists
                            if !generics.params.is_empty() {
                                generics.params.pop();
                            }
                        }
                    }
                }
            }
            if let syn::Item::Trait(trait_item) = item {
                let mut rng = thread_rng();
                let generics = &mut trait_item.generics;
                if rng.gen_bool(0.5) {
                    // Add a new generic parameter
                    generics.params.push(parse_quote!(const N: usize));
                } else {
                    // Remove the last generic parameter if it exists
                    if !generics.params.is_empty() {
                        generics.params.pop();
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the number of generic parameters in function, method, and trait signatures. It randomly adds or removes a generic parameter, testing the compiler's handling of different numbers of type parameters and const generics."
    }
}