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
    ItemTrait, TraitItem,
};

use crate::mutator::Mutator;

pub struct Modify_Lifetime_Parameters_In_Trait_Methods_38;

impl Mutator for Modify_Lifetime_Parameters_In_Trait_Methods_38 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Trait_Methods_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        let mut has_lifetime = false;
                        for param in &method.sig.generics.params {
                            if let syn::GenericParam::Lifetime(_) = param {
                                has_lifetime = true;
                                break;
                            }
                        }
                        if has_lifetime {
                            let new_lifetime: Lifetime = parse_quote!('b);
                            method.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: new_lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                            if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                                if let Type::Path(TypePath { path, .. }) = &mut **ty {
                                    if let Some(last_segment) = path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            args.args.push(GenericArgument::Lifetime(new_lifetime));
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
        ""
    }
}