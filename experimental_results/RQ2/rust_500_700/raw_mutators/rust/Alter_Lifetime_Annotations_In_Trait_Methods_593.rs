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

pub struct Alter_Lifetime_Annotations_In_Trait_Methods_593;

impl Mutator for Alter_Lifetime_Annotations_In_Trait_Methods_593 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_In_Trait_Methods_593"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut new_lifetime = None;
                for generics_param in &trait_item.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = generics_param {
                        new_lifetime = Some(lifetime_param.lifetime.clone());
                        break;
                    }
                }
                if new_lifetime.is_none() {
                    new_lifetime = Some(Lifetime::new("'b", Span::call_site()));
                    trait_item.generics.params.push(syn::GenericParam::Lifetime(LifetimeDef {
                        attrs: Vec::new(),
                        lifetime: new_lifetime.clone().unwrap(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Receiver(receiver) = input {
                                if let Some((_, lifetime)) = &mut receiver.reference {
                                    *lifetime = Some(new_lifetime.clone().unwrap());
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