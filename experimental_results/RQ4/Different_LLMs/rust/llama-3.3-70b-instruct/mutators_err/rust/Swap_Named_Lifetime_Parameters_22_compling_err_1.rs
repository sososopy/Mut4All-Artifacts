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

pub struct Swap_Named_Lifetime_Parameters_22;

impl Mutator for Swap_Named_Lifetime_Parameters_22 {
    fn name(&self) -> &str {
        "Swap_Named_Lifetime_Parameters_22"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut lifetime_params: Vec<LifetimeParam> = func.sig.generics.lifetimes.clone().into_iter().collect();
                if lifetime_params.len() > 1 {
                    let mut rng = thread_rng();
                    let idx1 = rng.gen_range(0..lifetime_params.len());
                    let idx2 = rng.gen_range(0..lifetime_params.len());
                    lifetime_params.swap(idx1, idx2);
                    func.sig.generics.lifetimes = lifetime_params.into_iter().collect();
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut lifetime_params: Vec<LifetimeParam> = func.sig.generics.lifetimes.clone().into_iter().collect();
                        if lifetime_params.len() > 1 {
                            let mut rng = thread_rng();
                            let idx1 = rng.gen_range(0..lifetime_params.len());
                            let idx2 = rng.gen_range(0..lifetime_params.len());
                            lifetime_params.swap(idx1, idx2);
                            func.sig.generics.lifetimes = lifetime_params.into_iter().collect();
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the positions of two randomly chosen named lifetime parameters in function signatures. This transformation tests the compiler's ability to correctly infer and check lifetimes under altered parameter orders, potentially exposing bugs in the handling of named lifetimes, especially in async/await contexts."
    }
}