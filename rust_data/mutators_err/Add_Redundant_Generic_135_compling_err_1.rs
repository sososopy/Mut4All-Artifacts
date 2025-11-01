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

pub struct Add_Redundant_Generic_135;

impl Mutator for Add_Redundant_Generic_135 {
    fn name(&self) -> &str {
        "Add_Redundant_Generic_135"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    let mut has_const = false;
                    for attr in &item_impl.attrs {
                        if attr.path.is_ident("const") {
                            has_const = true;
                            break;
                        }
                    }
                    if has_const {
                        if let Type::Path(TypePath { path, .. }) = &*item_impl.self_ty {
                            if path.segments.len() == 1 {
                                let segment = &path.segments[0];
                                let ident = &segment.ident;
                                let new_generic: GenericParam = parse_quote! { T };
                                let mut new_generics = item_impl.generics.clone();
                                new_generics.params.push(new_generic);
                                let new_impl: ItemImpl = parse_quote! {
                                    impl<#new_generics> const #ident for #ident {}
                                };
                                *item_impl = new_impl;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait implementations, specifically those marked with the 'const' keyword. It adds a redundant generic parameter to the implementation, creating a conflicting generic implementation for a type that already has a specific implementation. This tests the compiler's handling of conflicts in specialization and trait resolution."
    }
}