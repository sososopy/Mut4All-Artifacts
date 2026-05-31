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
    TraitItem, ImplItem, TraitItemFn, ImplItemFn,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Definition_And_Implementation_315;

impl Mutator for Modify_Trait_Definition_And_Implementation_315 {
    fn name(&self) -> &str {
        "Modify_Trait_Definition_And_Implementation_315"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let new_method: TraitItemFn = parse_quote! {
                    fn new_method(&self) -> impl std::fmt::Debug;
                };
                item_trait.items.push(TraitItem::Fn(new_method));
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "ExampleTrait" {
                        let new_impl_method: ImplItemFn = parse_quote! {
                            fn new_method(&self) -> impl std::fmt::Debug { "example" }
                        };
                        item_impl.items.push(ImplItem::Fn(new_impl_method));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}