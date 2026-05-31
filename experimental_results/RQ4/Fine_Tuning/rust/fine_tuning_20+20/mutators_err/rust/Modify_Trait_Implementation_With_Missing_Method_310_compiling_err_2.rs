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

pub struct Modify_Trait_Implementation_With_Missing_Method_310;

impl Mutator for Modify_Trait_Implementation_With_Missing_Method_310 {
    fn name(&self) -> &str {
        "Modify_Trait_Implementation_With_Missing_Method_310"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_map: HashMap<String, Vec<String>> = HashMap::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let trait_name = item_trait.ident.to_string();
                let mut method_names = Vec::new();
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        method_names.push(method.sig.ident.to_string());
                    }
                }
                trait_map.insert(trait_name, method_names);
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let trait_name = path.segments.last().unwrap().ident.to_string();
                    if let Some(method_names) = trait_map.get(&trait_name) {
                        let mut impl_methods = HashSet::new();
                        for item in &item_impl.items {
                            if let ImplItem::Fn(method) = item {
                                impl_methods.insert(method.sig.ident.to_string());
                            }
                        }
                        let missing_methods: Vec<_> = method_names
                            .iter()
                            .filter(|name| !impl_methods.contains(*name))
                            .collect();
                        if !missing_methods.is_empty() {
                            continue;
                        }
                        if !item_impl.items.is_empty() {
                            item_impl.items.pop();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations by removing method definitions that are required by the trait. It first collects all trait names and their method signatures, then identifies trait implementations and checks if any required methods are missing. If a complete implementation is found, it removes one method to trigger a missing method error, testing the compiler's handling of incomplete trait implementations."
    }
}