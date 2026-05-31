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

pub struct Modify_HRTB_Lifetimes_592;

impl Mutator for Modify_HRTB_Lifetimes_592 {
    fn name(&self) -> &str {
        "Modify_HRTB_Lifetimes_592"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(bound_lifetimes) = &mut trait_bound.lifetimes {
                                        for lifetime_def in &mut bound_lifetimes.lifetimes {
                                            if let GenericParam::Lifetime(lifetime_param) = lifetime_def {
                                                if lifetime_param.lifetime.ident == "b" {
                                                    lifetime_param.lifetime.ident = Ident::new("c", Span::call_site());
                                                }
                                            }
                                        }
                                    } else {
                                        let new_lifetime: Lifetime = parse_quote!('c);
                                        trait_bound.lifetimes = Some(BoundLifetimes {
                                            for_token: Default::default(),
                                            lt_token: token::Lt::default(),
                                            lifetimes: {
                                                let mut lifetimes = Punctuated::new();
                                                lifetimes.push(GenericParam::Lifetime(LifetimeParam::new(new_lifetime)));
                                                lifetimes
                                            },
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with HRTBs in their type signatures, specifically altering the lifetimes involved. By modifying existing lifetimes or introducing new ones, it tests the compiler's ability to handle lifetime inference and resolution in complex type scenarios, potentially uncovering issues related to lifetime handling in HRTB contexts."
    }
}