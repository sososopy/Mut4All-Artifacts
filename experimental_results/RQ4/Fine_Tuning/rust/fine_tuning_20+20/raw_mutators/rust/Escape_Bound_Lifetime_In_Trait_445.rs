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

pub struct Escape_Bound_Lifetime_In_Trait_445;

impl Mutator for Escape_Bound_Lifetime_In_Trait_445 {
    fn name(&self) -> &str {
        "Escape_Bound_Lifetime_In_Trait_445"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let has_hrtb = trait_item
                    .generics
                    .params
                    .iter()
                    .any(|param| matches!(param, GenericParam::Lifetime(_)));
                if !has_hrtb {
                    continue;
                }
                let new_lifetime = Lifetime::new("'x", Span::call_site());
                trait_item
                    .generics
                    .params
                    .push(GenericParam::Lifetime(LifetimeParam::new(
                        new_lifetime.clone(),
                    )));
                for item in &mut trait_item.items {
                    match item {
                        TraitItem::Type(type_item) => {
                            if let Some(bound_lifetimes) = &mut type_item.generics.params.iter_mut().find_map(|param| {
                                if let GenericParam::Lifetime(lifetime_param) = param {
                                    Some(lifetime_param)
                                } else {
                                    None
                                }
                            }) {
                                bound_lifetimes
                                    .bounds
                                    .push(Lifetime::new("'x", Span::call_site()));
                            }
                        }
                        TraitItem::Method(method) => {
                            if let Some(receiver) = method.sig.receiver() {
                                if let syn::FnArg::Receiver(receiver) = receiver {
                                    receiver.reference = Some((
                                        token::And {
                                            spans: [Span::call_site()],
                                        },
                                        Some(new_lifetime.clone()),
                                    ));
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait definitions with higher-ranked trait bounds (HRTBs) and introduces a new lifetime parameter. It then modifies associated types and methods to use this new lifetime in a way that potentially escapes the original bound context, aiming to expose compiler bugs related to lifetime resolution and well-formedness checks."
    }
}