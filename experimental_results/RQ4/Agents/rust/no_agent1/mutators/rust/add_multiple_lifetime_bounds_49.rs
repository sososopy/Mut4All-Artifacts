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

pub struct Add_Multiple_Lifetime_Bounds_49;

impl Mutator for Add_Multiple_Lifetime_Bounds_49 {
    fn name(&self) -> &str {
        "Add_Multiple_Lifetime_Bounds_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let lifetime_a = Lifetime::new("'a", Span::call_site());
                let lifetime_b = Lifetime::new("'b", Span::call_site());
                
                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime_a.clone(),
                    colon_token: Some(token::Colon::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_b.clone());
                        bounds
                    },
                }));

                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime_b.clone(),
                    colon_token: Some(token::Colon::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_a.clone());
                        bounds
                    },
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let lifetime_a = Lifetime::new("'a", Span::call_site());
                        let lifetime_b = Lifetime::new("'b", Span::call_site());
                        
                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_a.clone(),
                            colon_token: Some(token::Colon::default()),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(lifetime_b.clone());
                                bounds
                            },
                        }));

                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_b.clone(),
                            colon_token: Some(token::Colon::default()),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(lifetime_a.clone());
                                bounds
                            },
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds multiple lifetime bounds to function and method signatures, creating cyclic dependencies between lifetimes. This transformation is designed to test the compiler's handling of lifetime resolution and cyclic lifetime constraints, potentially triggering ICEs or other deep compiler bugs related to lifetime inference and checking."
    }
}