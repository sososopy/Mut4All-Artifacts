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

pub struct Modify_Lifetime_Annotations_In_Trait_Methods_191;

impl Mutator for Modify_Lifetime_Annotations_In_Trait_Methods_191 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Annotations_In_Trait_Methods_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_lifetime_method = false;
                for trait_item in &item_trait.items {
                    if let TraitItem::Fn(trait_fn) = trait_item {
                        if !trait_fn.sig.generics.params.is_empty() {
                            has_lifetime_method = true;
                            break;
                        }
                    }
                }
                if !has_lifetime_method {
                    continue;
                }
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Fn(trait_fn) = trait_item {
                        let mut lifetimes: Vec<Lifetime> = vec![];
                        for param in &trait_fn.sig.generics.params {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                lifetimes.push(lifetime_param.lifetime.clone());
                            }
                        }
                        if lifetimes.len() < 2 {
                            continue;
                        }
                        trait_fn.sig.generics.params.clear();
                        for lifetime in &lifetimes {
                            trait_fn
                                .sig
                                .generics
                                .params
                                .push(GenericParam::Lifetime(LifetimeParam {
                                    attrs: vec![],
                                    lifetime: lifetime.clone(),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));
                        }
                        if let ReturnType::Type(_, return_type) = &mut trait_fn.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                let mut new_bounds = Punctuated::new();
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let mut new_bound = trait_bound.clone();
                                        if let Some(lifetimes) = &trait_bound.lifetimes {
                                            let mut new_lifetimes = lifetimes.clone();
                                            new_lifetimes.lifetimes.clear();
                                            for lifetime in &lifetimes.lifetimes {
                                                new_lifetimes
                                                    .lifetimes
                                                    .push(LifetimeParam::new(lifetime.lifetime.clone()));
                                            }
                                            new_bound.lifetimes = Some(new_lifetimes);
                                        }
                                        new_bounds.push(TypeParamBound::Trait(new_bound));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                                type_impl_trait.bounds = new_bounds;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait methods with lifetime annotations by removing lifetime dependencies in the method signature and replacing them with independent lifetimes. It also modifies the return type's lifetime constraints to use a single lifetime from the method's generics. This transformation creates semantic lifetime conflicts while maintaining syntactic validity, aiming to stress-test the compiler's lifetime resolution and trait implementation systems."
    }
}