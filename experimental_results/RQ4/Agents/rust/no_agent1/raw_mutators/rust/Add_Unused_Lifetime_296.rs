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

pub struct Add_Unused_Lifetime_296;

impl Mutator for Add_Unused_Lifetime_296 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_296"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: vec![],
                        lifetime: unused_lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                            generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: vec![],
                                lifetime: unused_lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter ('unused) to non-main function and impl signatures. It aims to stress the compiler's lifetime resolution system by introducing lifetimes that are not utilized within the function body or signature, potentially triggering ICEs or unexpected behavior in lifetime inference and checking."
    }
}