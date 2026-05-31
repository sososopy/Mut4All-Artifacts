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

pub struct Add_Unused_Lifetimes_189;

impl Mutator for Add_Unused_Lifetimes_189 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime_a: Lifetime = parse_quote!('a);
                    let lifetime_b: Lifetime = parse_quote!('b);
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: vec![],
                        lifetime: lifetime_a.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: vec![],
                        lifetime: lifetime_b.clone(),
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
                            let lifetime_a: Lifetime = parse_quote!('a);
                            let lifetime_b: Lifetime = parse_quote!('b);
                            generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: vec![],
                                lifetime: lifetime_a.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                            generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: vec![],
                                lifetime: lifetime_b.clone(),
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
        "The mutation operator adds unused lifetimes 'a and 'b to the generics of non-main functions and methods. This transformation introduces redundant lifetime parameters, potentially leading to ICEs or bugs related to lifetime resolution and checking. It tests the compiler's handling of generics with excess lifetimes, which may interfere with the borrow checker or lifetime inference mechanisms."
    }
}