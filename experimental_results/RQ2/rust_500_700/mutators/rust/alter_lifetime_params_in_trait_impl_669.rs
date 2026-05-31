use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Alter_Lifetime_Params_In_Trait_Impl_669;

impl Mutator for Alter_Lifetime_Params_In_Trait_Impl_669 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Params_In_Trait_Impl_669"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let lifetimes = &mut method.sig.generics.params;
                        let mut new_lifetimes = Punctuated::new();
                        let mut lifetime_names: Vec<Ident> = vec![];
                        for param in lifetimes.iter() {
                            if let syn::GenericParam::Lifetime(lifetime) = param {
                                lifetime_names.push(lifetime.lifetime.ident.clone());
                            }
                        }
                        if lifetime_names.len() >= 2 {
                            for (i, lifetime) in lifetime_names.iter().enumerate() {
                                let new_lifetime: LifetimeParam = if i == 0 {
                                    parse_quote!('a)
                                } else {
                                    let prev_lifetime = Ident::new(&format!("{}", (b'a' + (i - 1) as u8) as char), Span::call_site());
                                    parse_quote!('b: 'a)
                                };
                                new_lifetimes.push(GenericParam::Lifetime(new_lifetime));
                            }
                            method.sig.generics.params = new_lifetimes;
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