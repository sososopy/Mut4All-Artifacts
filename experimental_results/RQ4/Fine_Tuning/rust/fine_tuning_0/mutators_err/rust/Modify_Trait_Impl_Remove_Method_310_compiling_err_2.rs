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
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Impl_Remove_Method_310;

impl Mutator for Modify_Trait_Impl_Remove_Method_310 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Remove_Method_310"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut trait_methods = HashSet::new();

        // Collect all trait methods
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Method(method) = item {
                        trait_methods.insert(method.sig.ident.clone());
                    }
                }
            }
        }

        // Mutate trait implementations
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let path_ident = path.segments.last().unwrap().ident.clone();
                    let mut methods_to_remove = Vec::new();

                    // Collect methods to remove
                    for impl_item in &item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            if trait_methods.contains(&method.sig.ident) {
                                methods_to_remove.push(method.sig.ident.clone());
                            }
                        }
                    }

                    // Randomly remove one or more methods
                    if !methods_to_remove.is_empty() {
                        let num_to_remove = rng.gen_range(1..=methods_to_remove.len());
                        methods_to_remove.shuffle(&mut rng);
                        for ident in methods_to_remove.iter().take(num_to_remove) {
                            item_impl.items.retain(|impl_item| {
                                if let ImplItem::Fn(method) = impl_item {
                                    &method.sig.ident != ident
                                } else {
                                    true
                                }
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies trait implementations and removes one or more method implementations, which are required by the trait. This causes a compilation error due to missing method implementations, testing the compiler's handling of incomplete trait implementations."
    }
}