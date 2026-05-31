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

pub struct Alter_Lifetime_In_Trait_Impl_196;

impl Mutator for Alter_Lifetime_In_Trait_Impl_196 {
    fn name(&self) -> &str {
        "Alter_Lifetime_In_Trait_Impl_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, ref mut generics, _)) = item_impl.trait_ {
                    if let Some(lifetime_param) = generics.params.iter().find_map(|param| {
                        if let syn::GenericParam::Lifetime(lifetime) = param {
                            Some(lifetime.clone())
                        } else {
                            None
                        }
                    }) {
                        let new_lifetime = Lifetime::new("'b", Span::call_site());
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Type(impl_type) = impl_item {
                                if let syn::Type::Reference(type_reference) = &mut *impl_type.ty {
                                    if let Some(ref mut lifetime) = type_reference.lifetime {
                                        if *lifetime == lifetime_param.lifetime {
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}