use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng, prelude::IteratorRandom};
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

pub struct Insert_Yield_In_Impl_Trait_Function_38;

impl Mutator for Insert_Yield_In_Impl_Trait_Function_38 {
    fn name(&self) -> &str {
        "Insert_Yield_In_Impl_Trait_Function_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits_in_scope = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                traits_in_scope.insert(trait_item.ident.clone());
            }
        }
        let mut suitable_functions = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        suitable_functions.push(item_fn.sig.ident.clone());
                    }
                }
            }
        }
        if suitable_functions.is_empty() && traits_in_scope.len() > 0 {
            let random_trait = traits_in_scope.iter().choose(&mut thread_rng()).unwrap();
            let new_fn: ItemFn = parse_quote! {
                fn new_fn() -> impl #random_trait {
                    yield;
                }
            };
            file.items.push(Item::Fn(new_fn));
        } else {
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                        if let Type::ImplTrait(_) = &**return_type {
                            let yield_stmt: Stmt = parse_quote! { yield; };
                            item_fn.block.stmts.insert(0, yield_stmt);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a yield expression within functions returning impl Trait without a generator body. It first collects traits present in the program to ensure trait availability. If no suitable function exists, it creates a new function with impl Trait return using a random trait from scope and includes yield. Otherwise, it prepends yield; to the body of each impl Trait returning function. This transformation introduces generator-like syntax in non-generator contexts, testing the compiler's handling of yield in unexpected places and its interaction with impl Trait return type inference."
    }
}