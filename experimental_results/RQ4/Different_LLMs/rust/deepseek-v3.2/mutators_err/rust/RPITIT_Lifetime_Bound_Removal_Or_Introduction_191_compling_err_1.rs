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

pub struct RPITIT_Lifetime_Bound_Removal_Or_Introduction_191;

impl Mutator for RPITIT_Lifetime_Bound_Removal_Or_Introduction_191 {
    fn name(&self) -> &str {
        "RPITIT_Lifetime_Bound_Removal_Or_Introduction_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(trait_method) = trait_item {
                        let sig = &mut trait_method.sig;
                        if let ReturnType::Type(_, return_type) = &sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let lifetime_params = sig.generics.params.iter()
                                    .filter_map(|param| match param {
                                        GenericParam::Lifetime(lifetime_param) => Some(&lifetime_param.lifetime),
                                        _ => None,
                                    })
                                    .collect::<Vec<_>>();
                                if lifetime_params.is_empty() {
                                    continue;
                                }
                                let mut rng = thread_rng();
                                let should_remove = rng.gen_bool(0.5);
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                if should_remove {
                                    new_bounds = new_bounds.iter()
                                        .filter(|bound| {
                                            match bound {
                                                TypeParamBound::Lifetime(lifetime) => false,
                                                _ => true,
                                            }
                                        })
                                        .cloned()
                                        .collect::<Punctuated<_, Plus>>();
                                } else {
                                    let existing_lifetime_bound = new_bounds.iter()
                                        .any(|bound| matches!(bound, TypeParamBound::Lifetime(_)));
                                    if !existing_lifetime_bound {
                                        let chosen_lifetime = lifetime_params.choose(&mut rng).unwrap();
                                        new_bounds.push(TypeParamBound::Lifetime(chosen_lifetime.clone()));
                                    }
                                }
                                trait_method.sig.output = ReturnType::Type(
                                    token::RArrow::default(),
                                    Box::new(Type::ImplTrait(TypeImplTrait {
                                        impl_token: type_impl_trait.impl_token.clone(),
                                        bounds: new_bounds,
                                    }))
                                );
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method signatures using RPITIT (return position impl trait in trait) that include explicit lifetime bounds on the return type. It modifies the lifetime bound by either removing it or introducing a new lifetime bound if none exists, using lifetimes already present in the method signature. This transformation aims to test the compiler's handling of lifetime bounds in RPITIT, potentially triggering ICEs or lifetime resolution errors."
    }
}