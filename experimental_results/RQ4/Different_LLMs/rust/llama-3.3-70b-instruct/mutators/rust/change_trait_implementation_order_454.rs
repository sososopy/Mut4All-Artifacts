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

pub struct Change_Trait_Implementation_Order_454;

impl Mutator for Change_Trait_Implementation_Order_454 {
    fn name(&self) -> &str {
        "Change_Trait_Implementation_Order_454"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls: Vec<ItemImpl> = Vec::new();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                trait_impls.push(item_impl.clone());
            }
        }

        if trait_impls.len() > 1 {
            // Shuffle the trait implementations
            let mut rng = thread_rng();
            trait_impls.shuffle(&mut rng);

            // Replace the original trait implementations with the shuffled ones
            let mut new_items: Vec<Item> = Vec::new();
            for item in &mut file.items {
                if let Item::Impl(_) = item {
                    continue;
                } else {
                    new_items.push(item.clone());
                }
            }
            for impl_item in trait_impls {
                new_items.push(Item::Impl(impl_item));
            }
            file.items = new_items;
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator changes the order of trait implementations in the code. It collects all trait implementations, shuffles them, and then replaces the original implementations with the shuffled ones. This transformation can help test the compiler's handling of trait coherence, specialization, and method resolution."
    }
}