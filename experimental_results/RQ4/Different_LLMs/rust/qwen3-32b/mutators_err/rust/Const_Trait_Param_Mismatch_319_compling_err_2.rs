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

pub struct Const_Trait_Param_Mismatch_319;

impl Mutator for Const_Trait_Param_Mismatch_319 {
    fn name(&self) -> &str {
        "Const_Trait_Param_Mismatch_319"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let has_type_params = generics.params.iter().any(|param| matches!(param, GenericParam::Type(_)));
                        if !has_type_params {
                            continue;
                        }
                        let mut new_params = Punctuated::new();
                        let mut lifetimes = Vec::new();
                        let mut counter = 0;
                        let base = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'];
                        
                        for param in &mut generics.params {
                            if let GenericParam::Type(type_param) = param {
                                let letter = base[counter % base.len()];
                                counter += 1;
                                let ident = format!("'{}", letter);
                                let new_lt = LifetimeParam {
                                    lifetime: Lifetime::new(&ident, Span::call_site()),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                };
                                new_params.push(GenericParam::Lifetime(new_lt));
                                lifetimes.push(ident);
                            } else if let GenericParam::Lifetime(lt_param) = param {
                                new_params.push(param.clone());
                            } else {
                                new_params.push(param.clone());
                            }
                        }
                        
                        let num_new_lifetimes = 1;
                        for i in 0..num_new_lifetimes {
                            let letter = base[counter % base.len()];
                            counter += 1;
                            let ident = format!("'{}", letter);
                            let new_lt = LifetimeParam {
                                lifetime: Lifetime::new(&ident, Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            new_params.push(GenericParam::Lifetime(new_lt));
                            lifetimes.push(ident);
                        }
                        
                        generics.params = new_params;
                        
                        let mut new_bounds = Punctuated::new();
                        if lifetimes.len() >= 2 {
                            let first = &lifetimes[0];
                            let second = &lifetimes[1];
                            let bound = parse_quote!(#second: #first);
                            new_bounds.push(bound);
                        }
                        
                        let where_clause = generics.where_clause.get_or_insert(WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        });
                        where_clause.predicates.extend(new_bounds);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type parameters in methods of const impl blocks with lifetime parameters and adds arbitrary lifetime parameters with bounds. This creates a parameter count mismatch between the trait method (which has type parameters) and the impl method (which has lifetimes), potentially triggering ICEs when const evaluation is attempted due to invalid parameter types."
    }
}