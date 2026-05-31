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

pub struct Modify_Trait_Bounds_441;

impl Mutator for Modify_Trait_Bounds_441 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_441"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if let Some(colon_token) = &item_trait.colon_token {
                    let mut new_bounds = Punctuated::new();
                    for bound in &item_trait.supertraits {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            if trait_bound.lifetimes.is_some() {
                                new_bounds.push(bound.clone());
                            } else {
                                let new_bound = TypeParamBound::Trait(TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    modifier: trait_bound.modifier.clone(),
                                    lifetimes: Some(BoundLifetimes {
                                        for_token: token::For {
                                            span: Span::call_site(),
                                        },
                                        lt_token: token::Lt {
                                            spans: [Span::call_site()],
                                        },
                                        lifetimes: {
                                            let mut lifetimes = Punctuated::new();
                                            lifetimes.push(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: Lifetime::new("'a", Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            });
                                            lifetimes
                                        },
                                        gt_token: token::Gt {
                                            spans: [Span::call_site()],
                                        },
                                    }),
                                    path: trait_bound.path.clone(),
                                });
                                new_bounds.push(new_bound);
                            }
                        } else {
                            new_bounds.push(bound.clone());
                        }
                    }
                    item_trait.supertraits = new_bounds;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with higher-ranked trait bounds (HRTBs) by replacing non-lifetime HRTBs with a specific lifetime, such as `for<'a>`, in the supertrait bounds. It aims to stress-test the compiler's handling of lifetimes in trait bounds, potentially exposing issues in lifetime resolution, variance, and trait system interactions."
    }
}