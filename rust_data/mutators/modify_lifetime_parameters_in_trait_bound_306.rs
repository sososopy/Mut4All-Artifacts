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

pub struct Modify_Lifetime_Parameters_In_Trait_Bound_306;

impl Mutator for Modify_Lifetime_Parameters_In_Trait_Bound_306 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Trait_Bound_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut lifetimes: Vec<&LifetimeParam> = item_fn.sig.generics.params.iter().filter_map(|param| {
                    if let GenericParam::Lifetime(lifetime) = param {
                        Some(lifetime)
                    } else {
                        None
                    }
                }).collect();

                if lifetimes.len() >= 2 {
                    lifetimes.swap(0, 1);

                    for param in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(PatType { ty, .. }) = param {
                            if let Type::ImplTrait(TypeImplTrait { bounds, .. }) = &mut **ty {
                                for bound in bounds.iter_mut() {
                                    if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                                        if let Some(last_segment) = path.segments.last_mut() {
                                            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                                for arg in args.args.iter_mut() {
                                                    if let GenericArgument::Lifetime(lt) = arg {
                                                        if lt == &lifetimes[0].lifetime {
                                                            *lt = lifetimes[1].lifetime.clone();
                                                        } else if lt == &lifetimes[1].lifetime {
                                                            *lt = lifetimes[0].lifetime.clone();
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if let ReturnType::Type(_, ref mut output_ty) = item_fn.sig.output {
                        if let Type::ImplTrait(TypeImplTrait { bounds, .. }) = &mut **output_ty {
                            for bound in bounds.iter_mut() {
                                if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                                    if let Some(last_segment) = path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                            for arg in args.args.iter_mut() {
                                                if let GenericArgument::Lifetime(lt) = arg {
                                                    if lt == &lifetimes[0].lifetime {
                                                        *lt = lifetimes[1].lifetime.clone();
                                                    } else if lt == &lifetimes[1].lifetime {
                                                        *lt = lifetimes[0].lifetime.clone();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with lifetime parameters in trait bounds. It swaps the order of lifetime parameters in the function's signature, ensuring the function's return type and parameter types reference the swapped lifetimes correctly. This mutation can lead to lifetime mismatches, potentially triggering compiler bugs related to lifetime substitution and region parameter handling."
    }
}