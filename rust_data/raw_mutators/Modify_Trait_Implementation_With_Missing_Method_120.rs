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

pub struct Modify_Trait_Implementation_With_Missing_Method_120;

impl Mutator for Modify_Trait_Implementation_With_Missing_Method_120 {
    fn name(&self) -> &str {
        "Modify_Trait_Implementation_With_Missing_Method_120"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods = HashSet::new();
        let mut impls_to_modify = Vec::new();

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let TraitItem::Method(method) = item {
                        trait_methods.insert(method.sig.ident.clone());
                    }
                }
            }
        }

        for item in &file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    let mut methods_to_remove = Vec::new();
                    for item in &impl_item.items {
                        if let ImplItem::Method(method) = item {
                            if trait_methods.contains(&method.sig.ident) {
                                methods_to_remove.push(method.sig.ident.clone());
                            }
                        }
                    }
                    if !methods_to_remove.is_empty() {
                        impls_to_modify.push((path.clone(), methods_to_remove));
                    }
                }
            }
        }

        for (path, methods_to_remove) in impls_to_modify {
            for item in &mut file.items {
                if let Item::Impl(impl_item) = item {
                    if let Some((_, impl_path, _)) = &impl_item.trait_ {
                        if *impl_path == path {
                            impl_item.items.retain(|item| {
                                if let ImplItem::Method(method) = item {
                                    !methods_to_remove.contains(&method.sig.ident)
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
        "The mutation operator targets trait implementations by removing required methods from the implementation. This results in an incomplete implementation, violating the trait's contract and causing a compiler error. The operator identifies trait definitions, collects their method signatures, and then finds corresponding implementations to remove matching methods, ensuring the mutation is comprehensive and effective."
    }
}