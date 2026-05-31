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

pub struct Modify_Trait_Method_Signature_With_Lifetime_Annotations_303;

impl Mutator for Modify_Trait_Method_Signature_With_Lifetime_Annotations_303 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_With_Lifetime_Annotations_303"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_async_fn = false;
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        if method.sig.asyncness.is_some() {
                            has_async_fn = true;
                            break;
                        }
                    }
                }
                if has_async_fn {
                    let mut has_lifetime = false;
                    for param in &item_trait.generics.params {
                        if let GenericParam::Lifetime(_) = param {
                            has_lifetime = true;
                            break;
                        }
                    }
                    if !has_lifetime {
                        item_trait
                            .generics
                            .params
                            .push(GenericParam::Lifetime(parse_quote!('a)));
                    }
                    for item in &mut item_trait.items {
                        if let TraitItem::Fn(method) = item {
                            if method.sig.asyncness.is_some() {
                                let mut has_lifetime = false;
                                for param in &method.sig.generics.params {
                                    if let GenericParam::Lifetime(_) = param {
                                        has_lifetime = true;
                                        break;
                                    }
                                }
                                if !has_lifetime {
                                    method
                                        .sig
                                        .generics
                                        .params
                                        .push(GenericParam::Lifetime(parse_quote!('a)));
                                }
                                for input in &mut method.sig.inputs {
                                    if let FnArg::Typed(pat_type) = input {
                                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                            type_ref.lifetime = Some(parse_quote!('a));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "State" {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if method.sig.ident == "execute" {
                                    for input in &mut method.sig.inputs {
                                        if let FnArg::Typed(pat_type) = input {
                                            if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                                type_ref.lifetime = Some(parse_quote!('a));
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async trait methods by introducing or modifying lifetime annotations in their signatures. It ensures that the trait and method signatures are consistent with lifetime parameters, particularly focusing on async functions within traits. This transformation stresses the compiler's handling of async trait method lifetimes, aiming to uncover issues related to lifetime resolution and async trait desugaring."
    }
}