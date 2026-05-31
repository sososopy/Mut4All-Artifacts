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

pub struct Swap_Async_ImplTrait_Lifetimes_153;

impl Mutator for Swap_Async_ImplTrait_Lifetimes_153 {
    fn name(&self) -> &str {
        "Swap_Async_ImplTrait_Lifetimes_153"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }
                let mut has_input_1 = false;
                let mut has_output_1 = false;
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(_) = &*pat_type.ty {
                            has_input_1 = true;
                            break;
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        has_output_1 = true;
                    }
                }
                if !has_input_1 || !has_output_1 {
                    continue;
                }
                let lifetime_params = item_fn.sig.generics.params.iter().filter(|p| matches!(p, GenericParam::Lifetime(_))).count();
                if lifetime_params == 0 {
                    item_fn.sig.generics.params.push(parse_quote!('a));
                    item_fn.sig.generics.params.push(parse_quote!('b));
                } else if lifetime_params == 1 {
                    let new_lifetime = Ident::new("b", Span::call_site());
                    let lifetime_param = LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new(&format!("'{}", new_lifetime), Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    };
                    item_fn.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                }
                let lifetime_params = item_fn.sig.generics.params.iter().filter_map(|p| match p {
                    GenericParam::Lifetime(lp) => Some(&lp.lifetime),
                    _ => None,
                }).collect::<Vec<_>>();
                if lifetime_params.len() < 2 {
                    continue;
                }
                let mut rng = thread_rng();
                let first_idx = rng.gen_range(0..lifetime_params.len());
                let second_idx = rng.gen_range(0..lifetime_params.len());
                let first_lifetime = lifetime_params[first_idx];
                let second_lifetime = lifetime_params[second_idx];
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            for bound in &mut type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(ref mut lifetimes) = trait_bound.lifetimes {
                                        if lifetimes.lifetimes.len() > 0 {
                                            if let GenericParam::Lifetime(lp) = &mut lifetimes.lifetimes[0] {
                                                lp.lifetime = second_lifetime.clone();
                                            }
                                        } else {
                                            lifetimes.lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: second_lifetime.clone(),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            }));
                                        }
                                    } else {
                                        trait_bound.lifetimes = Some(BoundLifetimes {
                                            for_token: token::For::default(),
                                            lt_token: token::Lt::default(),
                                            lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: second_lifetime.clone(),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            })]),
                                            gt_token: token::Gt::default(),
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(ref mut lifetimes) = trait_bound.lifetimes {
                                    if lifetimes.lifetimes.len() > 0 {
                                        if let GenericParam::Lifetime(lp) = &mut lifetimes.lifetimes[0] {
                                            lp.lifetime = first_lifetime.clone();
                                        }
                                    } else {
                                        lifetimes.lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: first_lifetime.clone(),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                    }
                                } else {
                                    trait_bound.lifetimes = Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: first_lifetime.clone(),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        })]),
                                        gt_token: token::Gt::default(),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions that use impl trait in both parameter and return position. It ensures the function has at least two lifetime parameters, then swaps or mismatches the lifetimes used in the input and output impl trait bounds. This creates a scenario where the compiler may attempt to substitute a region parameter that is out of range due to mismatched lifetime indices, potentially triggering bugs in lifetime handling for async functions with impl trait."
    }
}