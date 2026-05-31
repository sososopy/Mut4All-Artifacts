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

pub struct Introduce_Recursive_Const_Trait_Bound_320;

impl Mutator for Introduce_Recursive_Const_Trait_Bound_320 {
    fn name(&self) -> &str {
        "Introduce_Recursive_Const_Trait_Bound_320"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        let mut concrete_impls = HashSet::new();
        let mut generic_impls = Vec::new();
        
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_names.insert(item_trait.ident.to_string());
            }
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                    if item_impl.generics.params.is_empty() {
                        concrete_impls.insert(trait_name.clone());
                    } else {
                        generic_impls.push((item_impl.clone(), trait_name));
                    }
                }
            }
        }
        
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if !item_trait.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    item_trait.attrs.push(parse_quote!(#[const_trait]));
                }
            }
        }
        
        for (impl_item, trait_name) in generic_impls {
            if concrete_impls.contains(&trait_name) {
                if let Item::Impl(existing_impl) = file.items.iter_mut().find(|item| {
                    if let Item::Impl(item_impl) = item {
                        if let Some((_, trait_path, _)) = &item_impl.trait_ {
                            trait_path.segments.last().unwrap().ident.to_string() == trait_name
                        } else {
                            false
                        }
                    } else {
                        false
                    }
                }).unwrap() {
                    existing_impl.constness = Some(token::Const::default());
                    
                    let recursive_bound = parse_quote!(T: ~const #trait_name);
                    if let Some(where_clause) = &mut existing_impl.generics.where_clause {
                        where_clause.predicates.push(recursive_bound);
                    } else {
                        existing_impl.generics.where_clause = Some(parse_quote!(where #recursive_bound));
                    }
                }
            }
        }
        
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if concrete_impls.contains(&trait_path.segments.last().unwrap().ident.to_string()) {
                        item_impl.constness = Some(token::Const::default());
                    }
                }
            }
        }
        
        let has_feature = file.items.iter().any(|item| {
            if let Item::Macro(item_macro) = item {
                if let Ok(macro_content) = item_macro.mac.parse_body::<syn::Expr>() {
                    if let Expr::Lit(expr_lit) = macro_content {
                        if let Lit::Str(lit_str) = &expr_lit.lit {
                            return lit_str.value().contains("const_trait_impl");
                        }
                    }
                }
            }
            false
        });
        
        if !has_feature {
            file.items.insert(0, parse_quote!(#![feature(const_trait_impl)]));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces recursive const trait bounds in generic implementations. It first identifies all traits and their implementations, marking traits as const_trait if not already. For each generic impl of a trait that has at least one concrete impl, it adds a const qualifier and a recursive where clause T: ~const TraitName. All impls for the trait are made const. If the crate lacks the const_trait_impl feature, it's added at the crate root. This creates complex recursive const bounds that stress the compiler's coherence checking and const trait resolution, potentially exposing bugs in inference variable handling with recursive const bounds."
    }
}