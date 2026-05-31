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

pub struct Add_Async_Trait_Lifetime_Mismatch_301;

impl Mutator for Add_Async_Trait_Lifetime_Mismatch_301 {
    fn name(&self) -> &str {
        "Add_Async_Trait_Lifetime_Mismatch_301"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_methods = std::collections::HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if let Some(asyncness) = &method.sig.asyncness {
                            let lifetime = syn::Lifetime::new("'a", Span::call_site());
                            let lifetime_param = syn::GenericParam::Lifetime(syn::LifetimeParam {
                                attrs: vec![],
                                lifetime,
                                colon_token: None,
                            });
                            method.sig.generics.params.push(lifetime_param);
                            modified_methods.insert(method.sig.ident.clone());
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        if modified_methods.contains(&method.sig.ident) {
                            if let Some(first_param) = method.sig.generics.params.iter().next() {
                                if let syn::GenericParam::Lifetime(_) = first_param {
                                    method.sig.generics.params.remove(0);
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