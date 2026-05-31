use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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
    TraitItem,
};

use crate::mutator::Mutator;

pub struct Add_Unelided_Lifetime_In_Trait_Methods_295;

impl Mutator for Add_Unelided_Lifetime_In_Trait_Methods_295 {
    fn name(&self) -> &str {
        "Add_Unelided_Lifetime_In_Trait_Methods_295"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut new_lifetime = None;
                for item in &mut trait_item.items {
                    if let TraitItem::Fn(method) = item {
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut *return_type {
                                let mut has_associated_type = false;
                                for bound in &mut type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.segments.iter().any(|seg| seg.ident == "AssociatedType") {
                                            has_associated_type = true;
                                            if new_lifetime.is_none() {
                                                new_lifetime = Some(Lifetime::new("'new_lifetime", Span::call_site()));
                                            }
                                            if let Some(lifetime) = &new_lifetime {
                                                trait_bound.lifetimes = Some(BoundLifetimes {
                                                    for_token: Default::default(),
                                                    lt_token: Default::default(),
                                                    lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                                        attrs: Vec::new(),
                                                        lifetime: lifetime.clone(),
                                                        colon_token: None,
                                                        bounds: Punctuated::new(),
                                                    })]),
                                                    gt_token: Default::default(),
                                                });
                                            }
                                        }
                                    }
                                }
                                if has_associated_type && new_lifetime.is_some() {
                                    let lifetime_param = LifetimeParam {
                                        attrs: Vec::new(),
                                        lifetime: new_lifetime.clone().unwrap(),
                                        colon_token: None,
                                        bounds: Punctuated::new(),
                                    };
                                    if !method.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(l) if l.lifetime == lifetime_param.lifetime)) {
                                        method.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
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
        ""
    }
}