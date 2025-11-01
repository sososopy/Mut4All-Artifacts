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

pub struct Modify_Const_Trait_Implementation_134;

impl Mutator for Modify_Const_Trait_Implementation_134 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Implementation_134"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if !trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    trait_item.attrs.push(parse_quote!(#[const_trait]));
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if impl_item.generics.constness.is_some() {
                    let has_non_const_method = impl_item.items.iter().any(|impl_item| {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            !method.sig.constness.is_some()
                        } else {
                            false
                        }
                    });

                    if !has_non_const_method {
                        if let Some(syn::ImplItem::Fn(method)) = impl_item.items.first_mut() {
                            method.sig.constness = None;
                        }
                    }

                    let new_method: syn::ImplItemFn = parse_quote! {
                        fn new_method<A: ~const MyTrait>(&self) -> i32 {
                            42
                        }
                    };

                    impl_item.items.push(syn::ImplItem::Fn(new_method));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies traits and their implementations to explore the const trait feature. It adds #[const_trait] to traits if missing, ensures at least one method in a const impl does not use ~const, and introduces a new method using ~const."
    }
}