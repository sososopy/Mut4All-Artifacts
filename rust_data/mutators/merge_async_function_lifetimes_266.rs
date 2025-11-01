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

pub struct Merge_Async_Function_Lifetimes_266;

impl Mutator for Merge_Async_Function_Lifetimes_266 {
    fn name(&self) -> &str {
        "Merge_Async_Function_Lifetimes_266"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut lifetimes: Vec<&Lifetime> = Vec::new();
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Lifetime(lifetime_def) = param {
                            lifetimes.push(&lifetime_def.lifetime);
                        }
                    }
                    
                    if lifetimes.len() > 1 {
                        let first_lifetime = lifetimes[0].clone();
                        for input in &mut item_fn.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                    if let Some(lifetime) = &type_ref.lifetime {
                                        if lifetimes.contains(&lifetime) {
                                            type_ref.lifetime = Some(first_lifetime.clone());
                                        }
                                    }
                                }
                            }
                        }
                        
                        if let ReturnType::Type(_, ty) = &mut item_fn.sig.output {
                            if let Type::Reference(type_ref) = &mut **ty {
                                if let Some(lifetime) = &type_ref.lifetime {
                                    if lifetimes.contains(&lifetime) {
                                        type_ref.lifetime = Some(first_lifetime.clone());
                                    }
                                }
                            }
                        }
                        
                        item_fn.sig.generics.params = item_fn.sig.generics.params.iter().filter_map(|param| {
                            if let GenericParam::Lifetime(lifetime_def) = param {
                                if lifetime_def.lifetime == first_lifetime {
                                    Some(param.clone())
                                } else {
                                    None
                                }
                            } else {
                                Some(param.clone())
                            }
                        }).collect();
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets async functions with multiple lifetime parameters, consolidating them into a single lifetime to explore potential inconsistencies in lifetime handling."
    }
}