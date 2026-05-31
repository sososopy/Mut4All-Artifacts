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

pub struct Modify_Trait_Bounds_With_Lifetimes_30;

impl Mutator for Modify_Trait_Bounds_With_Lifetimes_30 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_With_Lifetimes_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics;
                let mut new_lifetime = None;

                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &mut type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.lifetimes.is_none() {
                                    let lifetime_ident = Ident::new("'a", Span::call_site());
                                    let lifetime = Lifetime::new(&lifetime_ident.to_string(), Span::call_site());
                                    trait_bound.lifetimes = Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: lifetime.clone(),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }]),
                                        gt_token: token::Gt::default(),
                                    });
                                    new_lifetime = Some(lifetime);
                                }
                            }
                        }
                    }
                }

                if let Some(lifetime) = new_lifetime {
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime,
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}