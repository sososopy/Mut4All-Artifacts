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

pub struct Modify_Lifetime_Parameters_In_Trait_Bound_153;

impl Mutator for Modify_Lifetime_Parameters_In_Trait_Bound_153 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Trait_Bound_153"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_none() {
                    continue;
                }
                let mut lifetime_params = Vec::new();
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        lifetime_params.push(lifetime.clone());
                    }
                }
                if lifetime_params.len() < 2 {
                    continue;
                }
                lifetime_params.reverse();
                let mut new_generics = item_fn.sig.generics.clone();
                new_generics.params.clear();
                for param in &lifetime_params {
                    new_generics.params.push(GenericParam::Lifetime(param.clone()));
                }
                for param in &item_fn.sig.generics.params {
                    if !matches!(param, GenericParam::Lifetime(_)) {
                        new_generics.params.push(param.clone());
                    }
                }
                item_fn.sig.generics = new_generics;
                let mut new_inputs = Punctuated::new();
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let mut new_lifetimes = Vec::new();
                                    if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                                        for lifetime in &bound_lifetimes.lifetimes {
                                            new_lifetimes.push(lifetime.clone());
                                        }
                                    }
                                    if new_lifetimes.len() >= 2 {
                                        new_lifetimes.reverse();
                                    }
                                    let mut new_bound = trait_bound.clone();
                                    new_bound.lifetimes = Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        lifetimes: Punctuated::from_iter(new_lifetimes),
                                        gt_token: token::Gt::default(),
                                    });
                                    new_bounds.push(TypeParamBound::Trait(new_bound));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            let new_type_impl_trait = TypeImplTrait {
                                impl_token: token::Impl::default(),
                                bounds: new_bounds,
                            };
                            let new_ty = Type::ImplTrait(new_type_impl_trait);
                            new_inputs.push(FnArg::Typed(PatType {
                                attrs: pat_type.attrs.clone(),
                                pat: pat_type.pat.clone(),
                                colon_token: pat_type.colon_token,
                                ty: Box::new(new_ty),
                            }));
                        } else {
                            new_inputs.push(input.clone());
                        }
                    } else {
                        new_inputs.push(input.clone());
                    }
                }
                item_fn.sig.inputs = new_inputs;
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut new_lifetimes = Vec::new();
                                if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                                    for lifetime in &bound_lifetimes.lifetimes {
                                        new_lifetimes.push(lifetime.clone());
                                    }
                                }
                                if new_lifetimes.len() >= 2 {
                                    new_lifetimes.reverse();
                                }
                                let mut new_bound = trait_bound.clone();
                                new_bound.lifetimes = Some(BoundLifetimes {
                                    for_token: token::For::default(),
                                    lt_token: token::Lt::default(),
                                    lifetimes: Punctuated::from_iter(new_lifetimes),
                                    gt_token: token::Gt::default(),
                                });
                                new_bounds.push(TypeParamBound::Trait(new_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        let new_type_impl_trait = TypeImplTrait {
                            impl_token: token::Impl::default(),
                            bounds: new_bounds,
                        };
                        let new_ty = Type::ImplTrait(new_type_impl_trait);
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow::default(),
                            Box::new(new_ty),
                        );
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions with lifetime parameters in their trait bounds. It reverses the order of lifetime parameters in the function signature and within impl Trait bounds in both parameters and return types. By altering the expected lifetime associations, this transformation can induce lifetime resolution errors, testing the compiler's ability to handle lifetime parameterization and substitution in async contexts."
    }
}