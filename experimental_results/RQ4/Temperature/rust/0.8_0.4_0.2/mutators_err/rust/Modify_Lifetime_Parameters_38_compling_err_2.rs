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

pub struct Modify_Lifetime_Parameters_38;

impl Mutator for Modify_Lifetime_Parameters_38 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Trait(item_trait) => {
                    for item in &mut item_trait.items {
                        if let syn::TraitItem::Fn(method) = item {
                            if let Some(first_lifetime) = method.sig.generics.lifetimes().next() {
                                let new_lifetime: Lifetime = parse_quote!('x);
                                method.sig.generics.params.iter_mut().for_each(|param| {
                                    if let GenericParam::Lifetime(lifetime_param) = param {
                                        if lifetime_param.lifetime == first_lifetime.lifetime {
                                            lifetime_param.lifetime = new_lifetime.clone();
                                        }
                                    }
                                });
                            }
                        }
                    }
                }
                Item::Struct(item_struct) => {
                    if let Some(first_lifetime) = item_struct.generics.lifetimes().next() {
                        let new_lifetime: Lifetime = parse_quote!('static);
                        item_struct.generics.params.iter_mut().for_each(|param| {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                if lifetime_param.lifetime == first_lifetime.lifetime {
                                    lifetime_param.lifetime = new_lifetime.clone();
                                }
                            }
                        });
                        for field in &mut item_struct.fields {
                            if let Type::Path(TypePath { path, .. }) = &mut field.ty {
                                for segment in &mut path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Lifetime(lifetime) = arg {
                                                if *lifetime == first_lifetime.lifetime {
                                                    *lifetime = new_lifetime.clone();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Impl(item_impl) => {
                    if let Some(first_lifetime) = item_impl.generics.lifetimes().next() {
                        let new_lifetime: Lifetime = parse_quote!('d);
                        item_impl.generics.params.iter_mut().for_each(|param| {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                if lifetime_param.lifetime == first_lifetime.lifetime {
                                    lifetime_param.lifetime = new_lifetime.clone();
                                }
                            }
                        });
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Type(impl_type) = impl_item {
                                if let Some(first_lifetime) = impl_type.generics.lifetimes().next() {
                                    let new_lifetime: Lifetime = parse_quote!('e);
                                    impl_type.generics.params.iter_mut().for_each(|param| {
                                        if let GenericParam::Lifetime(lifetime_param) = param {
                                            if lifetime_param.lifetime == first_lifetime.lifetime {
                                                lifetime_param.lifetime = new_lifetime.clone();
                                            }
                                        }
                                    });
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}