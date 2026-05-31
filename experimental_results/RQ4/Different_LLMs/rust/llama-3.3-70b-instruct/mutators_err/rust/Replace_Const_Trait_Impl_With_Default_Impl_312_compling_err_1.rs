use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Trait_Impl_With_Default_Impl_312;

impl Mutator for Replace_Const_Trait_Impl_With_Default_Impl_312 {
    fn name(&self) -> &str {
        "Replace_Const_Trait_Impl_With_Default_Impl_312"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for attribute in &trait_item.attrs {
                    if attribute.path.is_ident("const_trait") {
                        trait_item.attrs.retain(|attr| !attr.path.is_ident("const_trait"));
                        for item_impl in &mut file.items {
                            if let Item::Impl(impl_item) = item_impl {
                                if let Some((_, path, _)) = impl_item.of {
                                    if path.is_ident(&trait_item.ident) {
                                        for impl_item in &mut impl_item.items {
                                            if let ImplItem::Method(method) = impl_item {
                                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                                        if let Some(const_trait_bound) = type_impl_trait.bounds.iter().find(|bound| {
                                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                                trait_bound.path.is_ident("const_trait")
                                                            } else {
                                                                false
                                                            }
                                                        }) {
                                                            let default_impl_trait = TypeImplTrait {
                                                                impl_token: type_impl_trait.impl_token,
                                                                bounds: type_impl_trait.bounds.clone().into_iter().filter(|bound| {
                                                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                                                        !trait_bound.path.is_ident("const_trait")
                                                                    } else {
                                                                        true
                                                                    }
                                                                }).collect(),
                                                            };
                                                            *return_type = Box::new(Type::ImplTrait(default_impl_trait));
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
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const trait implementations with default implementations, removing the const_trait feature. This transformation tests the compiler's ability to handle changes in trait implementations and ensures that the removal of const_trait does not cause internal compiler errors or unexpected behavior."
    }
}