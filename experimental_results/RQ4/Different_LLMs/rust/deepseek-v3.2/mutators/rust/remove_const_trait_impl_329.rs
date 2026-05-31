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

pub struct Remove_Const_Trait_Impl_329;

impl Mutator for Remove_Const_Trait_Impl_329 {
    fn name(&self) -> &str {
        "Remove_Const_Trait_Impl_329"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_impls = Vec::new();
        let mut trait_names = HashSet::new();
        
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        trait_names.insert(path.segments.last().unwrap().ident.to_string());
                    }
                }
            }
        }
        
        for (index, item) in file.items.iter_mut().enumerate() {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() && item_impl.unsafety.is_none() && item_impl.defaultness.is_none() {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        let trait_name = path.segments.last().unwrap().ident.to_string();
                        if trait_names.contains(&trait_name) {
                            const_impls.push(index);
                        }
                    }
                }
            }
        }
        
        if !const_impls.is_empty() {
            let mut rng = thread_rng();
            if let Some(&target_idx) = const_impls.choose(&mut rng) {
                if let Item::Impl(item_impl) = &mut file.items[target_idx] {
                    item_impl.unsafety = None;
                    item_impl.defaultness = None;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const trait implementations in the seed program and removes the const qualifier from one of them. It specifically targets const impls for traits that have at least one other implementation (const or non-const) elsewhere in the program. This transformation tests the compiler's handling of const trait resolution, particularly in generic contexts where ~const bounds may be involved. By converting a const impl to a regular impl, it may affect trait resolution in complex scenarios involving pin projections and generic type parameters."
    }
}